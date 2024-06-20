#include "message_man.h"
#include <QBuffer>
#include <QTcpSocket>
#include <stdexcept>

MessageMan::MessageMan(QObject* parent)
    : QObject(parent),
      firstWrite(true),
      writeBuf(new QBuffer(this)),
      writer(writeBuf),
      socket(nullptr) {
  writeBuf->open(QIODeviceBase::OpenModeFlag::WriteOnly);
  writer.setAutoFormatting(true);
}

void MessageMan::setPeer(QTcpSocket* peer) {
  if (isConnected()) {
    throw std::invalid_argument("release socket first");
  }
  socket = peer;
  connect(socket, &QTcpSocket::readyRead, this, &MessageMan::processMessage);
}

bool MessageMan::isConnected() const {
  return socket;
}

void MessageMan::release() {
  if (isConnected()) {
    disconnect(socket, &QTcpSocket::readyRead, this, &MessageMan::processMessage);
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

void MessageMan::sendMessage(QByteArray const& data) {
  sendMessage(data, data.size());
}

void MessageMan::sendMessage(QByteArray const& data, qsizetype size) {
  if (!isConnected()) {
    return;
  }
  qDebug() << "sending message...` size=" << size;
  qDebug() << "message: " << data << " (truncated to `size` bytes)";
  socket->write(sizeToBytes(size));
  socket->write(data, size);
}

QXmlStreamWriter& MessageMan::getStreamWriter() {
  // todo not connected
  if (firstWrite) {
    writer.writeStartDocument();
    firstWrite = false;
  }
  return writer;
}

void MessageMan::flushMessage() {
  writer.writeEndDocument();
  sendMessage(writeBuf->data(), writeBuf->pos());;
  writeBuf->reset();
  firstWrite = true;
}

void MessageMan::processMessage() {
  assert(socket);
  auto size = scanSize(socket);
  auto data = socket->read(size);
  qDebug() << "message received. size=" << size;
  qDebug() << "message: " << data;
  emit messageReady(data);
}
