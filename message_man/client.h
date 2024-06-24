#pragma once

#include <QBuffer>
#include <QXmlStreamWriter>

class MessageMan;

class Client {
public:
  Client() = delete;
  explicit Client(MessageMan const* messageMan);
  QXmlStreamWriter& getStreamWriter();
  void finishWrite();
  [[nodiscard]] QXmlStreamReader const& getStreamReader() const;
  [[nodiscard]] bool xmlReadNextStartElement() const;
  void xmlSkipCurrentElement() const;
  void finishRead() const;

private:
  MessageMan const* messageMan;
  bool firstWrite;
  QBuffer writeBuf;
  QXmlStreamWriter writer;
};
