#include "message_man.h"
#include <QTcpSocket>
#include <iostream>

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

void MessageMan::sendMessage(QByteArray const& data) {
  if (!isConnected()) {
    throw std::runtime_error("sendMessage: not connected");
  }
  std::cout << "sending message (" << data.size() << "): " << data.data() << std::endl;
  socket->write(QByteArray::number(data.size()));
  socket->write(data);
}

void MessageMan::processMessage() {
  assert(socket);
  auto size = socket->read(sizeof(qlonglong)).toLongLong();
  std::cout << "message received. size=" << size << std::endl;
  auto data = socket->read(size);
  std::cout << "message: " << data.data() << std::endl;
  emit messageReady(data);
}
