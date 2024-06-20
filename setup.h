#ifndef SETUP_H
#define SETUP_H

#include <QDialog>
#include <QAbstractSocket>

class MessageMan;
class QRadioButton;
class QTcpSocket;
class QTcpServer;
class QLabel;
class QLineEdit;

class Setup : public QDialog {
  Q_OBJECT

public:
  explicit Setup(MessageMan* messageMan, QWidget* parent = nullptr);

private:
  void reject() override;
  void closeEvent(QCloseEvent* event) override;

  enum class SetupMode { CLIENT, SERVER, LOCAL };
  [[nodiscard]] SetupMode getSetupMode() const;

  void clientSetup();
  void serverSetup();

signals:
  void setupFinished();

private slots:
  void onOptionChosen();
  void onNewConnection();
  void displayError(QAbstractSocket::SocketError socketError);
  void tryConnect();
  void finishClientSetup();

private:
  MessageMan* messageMan;
  QRadioButton* clientButton;
  QRadioButton* serverButton;
  QTcpSocket* client;
  QTcpServer* server;
  QLabel* infoLabel;
  QLineEdit* ipLineEdit;
};

#endif // SETUP_H
