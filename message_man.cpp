#include "message_man.h"
#include <QTcpSocket>
#include <stdexcept>

MessageMan::MessageMan(QObject* parent) : QObject(parent), socket(nullptr) {}

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
QByteArray sizeToBytes(QByteArray const& data) {
  auto buf = QByteArray{};
  auto sz = static_cast<quint16>(data.size());
  buf.append(static_cast<char>((sz >> 0) & 0xFF));
  buf.append(static_cast<char>((sz >> 8) & 0xFF));
  return buf;
}

qsizetype scanSize(QTcpSocket* socket) {
  auto c = char{};
  auto sz = quint16{};
  socket->getChar(&c);
  sz |= c << 0;
  socket->getChar(&c);
  sz |= c << 8;
  return sz;
}
} // namespace

void MessageMan::sendMessage(QByteArray const& data) {
  if (!isConnected()) {
    throw std::runtime_error("sendMessage: not connected");
  }
  qDebug() << "sending message... size=" << data.size();
  qDebug() << "message: " << data;
  socket->write(sizeToBytes(data));
  socket->write(data);
}

void MessageMan::processMessage() {
  assert(socket);
  auto size = scanSize(socket);
  auto data = socket->read(size);
  qDebug() << "message received. size=" << size;
  qDebug() << "message: " << data;
  emit messageReady(data);
}
