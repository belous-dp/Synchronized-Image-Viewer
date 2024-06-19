#ifndef SERVER_H
#define SERVER_H

#include <QObject>

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
  ~MessageMan() override;

signals:
  void messageReady(QByteArray const& data);

private slots:
  void processMessage();

private:
  QByteArray buf;
  QTcpSocket* socket;
};

#endif // SERVER_H
