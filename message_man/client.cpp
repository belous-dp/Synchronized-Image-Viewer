#include "client.h"

#include "message_man.h"

Client::Client(MessageMan const* messageMan)
    : messageMan(messageMan),
      firstWrite(true),
      writer(&writeBuf) {
  writeBuf.open(QIODeviceBase::OpenModeFlag::WriteOnly);
  writer.setAutoFormatting(true);
}

QXmlStreamWriter& Client::getStreamWriter() {
  // todo not connected
  if (firstWrite) {
    writer.writeStartDocument();
    firstWrite = false;
  }
  return writer;
}

void Client::finishWrite() {
  writer.writeEndDocument();
  messageMan->sendMessage(writeBuf.data(), writeBuf.pos());
  writeBuf.reset();
  firstWrite = true;
}

QXmlStreamReader const& Client::getStreamReader() const {
  return messageMan->getStreamReader();
}

void Client::finishRead() const {
  return messageMan->finishRead();
}

bool Client::xmlReadNextStartElement() const {
  return messageMan->xmlReadNextStartElement();
}

void Client::xmlSkipCurrentElement() const {
  return messageMan->xmlSkipCurrentElement();
}
