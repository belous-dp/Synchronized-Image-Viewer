#ifndef SERVER_H
#define SERVER_H

#include <QBuffer>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

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

  void sendMessage(QByteArray const& data);
  void sendMessage(QByteArray const& data, qsizetype size);

  // stream-based API
  QXmlStreamWriter& getStreamWriter(); // todo move to write_client
  void finishWrite(); // move to write_client
  // todo WriteClient getWriteClient()
  QXmlStreamReader& getStreamReader();
  void finishRead();




  ~MessageMan() override;

signals:
  void messageReady();

private slots:
  void processMessage();

private:
  bool firstWrite; // todo move to write_client
  bool firstRead;
  uint32_t leftRead;
  QBuffer* writeBuf; // move to write_client
  QBuffer readBuf;
  QXmlStreamWriter writer; // move to write_client
  QXmlStreamReader reader;
  QTcpSocket* socket;
};

#endif // SERVER_H
