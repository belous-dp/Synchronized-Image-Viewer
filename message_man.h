#ifndef SERVER_H
#define SERVER_H

#include <QBuffer>
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
  QXmlStreamWriter& getStreamWriter();
  void flushMessage();
  ~MessageMan() override;

signals:
  void messageReady(QByteArray const& data);

private slots:
  void processMessage();

private:
  bool firstWrite;
  QBuffer* writeBuf;
  QXmlStreamWriter writer;
  QTcpSocket* socket;
};

#endif // SERVER_H
