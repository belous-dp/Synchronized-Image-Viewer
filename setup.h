#ifndef SETUP_H
#define SETUP_H

#include <QAbstractSocket>
#include <QDialog>

class MessageMan;
class QRadioButton;
class QTcpSocket;
class QTcpServer;
class QLabel;
class QLineEdit;
class QPushButton;

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
  void finishSetup();

signals:
  void setupFinished();

private slots:
  void onOptionChosen();
  void displayError(QAbstractSocket::SocketError socketError);
  void tryConnect();
  void finishClientSetup();
  void finishServerSetup();

private:
  MessageMan* messageMan;
  QRadioButton* clientButton;
  QRadioButton* serverButton;
  QTcpSocket* client;
  QTcpServer* server;
  QLabel* infoLabel;
  QLineEdit* ipLineEdit;
  QPushButton* button;
};

#endif // SETUP_H
