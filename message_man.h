#ifndef SERVER_H
#define SERVER_H

#include <QObject>

class QTcpSocket;

class MessageMan : QObject {
  Q_OBJECT

public:
  MessageMan() = delete;
  MessageMan(MessageMan const&) = delete;
  explicit MessageMan(QTcpSocket* peer);
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
