#ifndef SETUP_H
#define SETUP_H

#include <QDialog>

class MessageMan;
class QRadioButton;
class QTcpServer;
class QLabel;

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

private:
  MessageMan* messageMan;
  QRadioButton* clientButton;
  QRadioButton* serverButton;
  QTcpServer* server;
  QLabel* infoLabel;
};

#endif // SETUP_H
