#include "message_man.h"
#include <QTcpSocket>
#include <iostream>

MessageMan::MessageMan(QTcpSocket* peer) : socket(peer) {
  connect(socket, &QIODevice::readyRead, this, &MessageMan::processMessage);
}

MessageMan::~MessageMan() {
  socket->disconnectFromHost();
}

void MessageMan::sendMessage(QByteArray const& data) {
  std::cout << "sending message (" << data.size() << "): " << data.data() << std::endl;
  socket->write(QByteArray::number(data.size()));
  socket->write(data);
}

void MessageMan::processMessage() {
  auto size = socket->read(sizeof(qlonglong)).toLongLong();
  std::cout << "message received. size=" << size << std::endl;
  auto data = socket->read(size);
  std::cout << "message: " << data.data() << std::endl;
  emit messageReady(data);
}
