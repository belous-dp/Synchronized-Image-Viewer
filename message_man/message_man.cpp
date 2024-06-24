#include "message_man.h"
#include <QBuffer>
#include <QTcpSocket>
#include <stdexcept>

MessageMan::MessageMan(QObject* parent)
    : QObject(parent),
      firstRead(true),
      leftRead(0),
      reader(&readBuf),
      socket(nullptr) {
  readBuf.open(QIODeviceBase::ReadOnly);
  reader.setNamespaceProcessing(false);
}

void MessageMan::setPeer(QTcpSocket* peer) {
  if (isConnected()) {
    throw std::invalid_argument("MessageMan::setPeer: release socket first.");
  }
  if (peer == nullptr) {
    throw std::invalid_argument("MessageMan::setPeer: peer must be non-null.");
  }
  socket = peer;
  connect(socket, &QTcpSocket::readyRead, this, &MessageMan::processMessage);
  connect(socket, &QTcpSocket::disconnected, this, &MessageMan::deleteSocket);
}

bool MessageMan::isConnected() const {
  return socket;
}

void MessageMan::release() {
  if (isConnected()) {
    disconnect(socket, &QTcpSocket::readyRead, this, &MessageMan::processMessage);
    disconnect(socket, &QTcpSocket::disconnected, this, &MessageMan::deleteSocket);
    socket = nullptr;
  }
}

MessageMan::~MessageMan() {
  if (isConnected()) {
    socket->disconnectFromHost();
  }
}

namespace {
QByteArray sizeToBytes(qsizetype size) {
  auto buf = QByteArray{};
  auto sz = static_cast<quint32>(size);
  buf.append(static_cast<char>((sz >> 0) & 0xFF));
  buf.append(static_cast<char>((sz >> 8) & 0xFF));
  buf.append(static_cast<char>((sz >> 16) & 0xFF));
  buf.append(static_cast<char>((sz >> 24) & 0xFF));
  return buf;
}

qsizetype scanSize(QTcpSocket* socket) {
  auto c = char{};
  auto sz = quint32{};
  socket->getChar(&c);
  sz |= c << 0;
  socket->getChar(&c);
  sz |= c << 8;
  socket->getChar(&c);
  sz |= c << 16;
  socket->getChar(&c);
  sz |= c << 24;
  return sz;
}
} // namespace

void MessageMan::sendMessage(QByteArray const& data) const {
  sendMessage(data, data.size());
}

void MessageMan::sendMessage(QByteArray const& data, qsizetype size) const {
  if (!isConnected()) {
    return;
  }
  qDebug() << "sending message of size" << size << ". message:" << data
           << (data.size() > size
                   ? "(except last " + QString::number(data.size() - size) + " byte(s))"
                   : "");
  socket->write(sizeToBytes(size));
  socket->write(data, size);
}

Client MessageMan::getClient() const {
  return Client(this);
}

void MessageMan::processMessage() {
  assert(socket);
  if (leftRead == 0) {
    leftRead = scanSize(socket);
    readBuf.buffer().resize(leftRead);
  }
  leftRead -= socket->read(readBuf.buffer().end() - leftRead, leftRead);
  qDebug() << "part of message received. full message size:" << readBuf.size()
           << ". current message:" << readBuf.data();
  if (leftRead == 0) {
    emit messageReady();
  }
}

void MessageMan::deleteSocket() {
  socket->deleteLater();
  socket = nullptr;
}

QXmlStreamReader const& MessageMan::getStreamReader() const {
  return reader;
}

bool MessageMan::xmlReadNextStartElement() const {
  return reader.readNextStartElement();
}

void MessageMan::xmlSkipCurrentElement() const {
  return reader.skipCurrentElement();
}

void MessageMan::finishRead() const {
  reader.clear();
  readBuf.reset();
  reader.setDevice(&readBuf);
}
