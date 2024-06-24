#ifndef SERVER_H
#define SERVER_H

#include "client.h"

#include <QBuffer>
#include <QXmlStreamReader>

class QBuffer;
class QTcpSocket;

class MessageMan : public QObject {
  Q_OBJECT

public:
  explicit MessageMan(QObject* parent = nullptr);
  MessageMan(MessageMan const&) = delete;
  void setPeer(QTcpSocket* peer);
  [[nodiscard]] bool isConnected() const;
  void release();

  void sendMessage(QByteArray const& data) const;
  void sendMessage(QByteArray const& data, qsizetype size) const;

  // stream-based API
  Client getClient() const;
  QXmlStreamReader const& getStreamReader() const;
  bool xmlReadNextStartElement() const;
  void xmlSkipCurrentElement() const;
  void finishRead() const;

  ~MessageMan() override;

signals:
  void messageReady() const;

private slots:
  void processMessage();
  void deleteSocket();

private:
  bool firstRead;
  uint32_t leftRead;
  mutable QBuffer readBuf;
  mutable QXmlStreamReader reader;
  QTcpSocket* socket;
};

#endif // SERVER_H
