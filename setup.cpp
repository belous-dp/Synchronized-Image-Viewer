#include "setup.h"
#include "message_man.h"
#include <QCloseEvent>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QPushButton>
#include <QRadioButton>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QVBoxLayout>
#include <chrono>

Setup::Setup(MessageMan* messageMan, QWidget* parent)
    : QDialog(parent),
      messageMan(messageMan),
      client(nullptr),
      server(nullptr),
      infoLabel(nullptr),
      ipLineEdit(nullptr) {
  clientButton = new QRadioButton(tr("&client mode"));
  serverButton = new QRadioButton(tr("&server mode"));
  connect(clientButton, &QRadioButton::clicked, this, &Setup::onOptionChosen);
  connect(serverButton, &QRadioButton::clicked, this, &Setup::onOptionChosen);
  clientButton->setMinimumSize(200, 30);
  serverButton->setMinimumSize(200, 30);
  auto layout = new QGridLayout{};
  layout->addWidget(clientButton);
  layout->addWidget(serverButton);
  setLayout(layout);
}

void Setup::reject() {
  close();
}

void Setup::closeEvent(QCloseEvent* event) {
  qDebug() << "closed";
  emit setupFinished();
  event->accept();
}

Setup::SetupMode Setup::getSetupMode() const {
  if (clientButton->isChecked()) {
    return SetupMode::CLIENT;
  }
  if (serverButton->isChecked()) {
    return SetupMode::SERVER;
  }
  return SetupMode::LOCAL;
}

void Setup::onOptionChosen() {
  auto mode = getSetupMode();
  delete clientButton;
  delete serverButton;
  switch (mode) {
  case SetupMode::CLIENT:
    clientSetup();
    break;
  case SetupMode::SERVER:
    serverSetup();
    break;
  default:
    Q_UNREACHABLE();
  }
}

void Setup::clientSetup() {
  qDebug() << "client mode";

  client = new QTcpSocket(messageMan);
  connect(client, &QTcpSocket::errorOccurred, this, &Setup::displayError);
  connect(client, &QTcpSocket::connected, this, &Setup::finishClientSetup);

  auto ipLabel = new QLabel{tr("&server ip:port")};
  ipLabel->setMinimumWidth(100);
  ipLineEdit = new QLineEdit{};
  ipLineEdit->setMinimumWidth(150);
  ipLabel->setBuddy(ipLineEdit);

  infoLabel = new QLabel{tr("waiting for server address...")};

  auto connectButton = new QPushButton{tr("Connect")};
  connect(connectButton, &QPushButton::clicked, this, &Setup::tryConnect);

  dynamic_cast<QGridLayout*>(layout())->addWidget(ipLabel, 0, 0);
  dynamic_cast<QGridLayout*>(layout())->addWidget(ipLineEdit, 0, 1);
  dynamic_cast<QGridLayout*>(layout())->addWidget(infoLabel, 1, 0, 1, 2);
  dynamic_cast<QGridLayout*>(layout())->addWidget(connectButton, 2, 0, 1, 2);

  ipLineEdit->setFocus();
}

void Setup::tryConnect() {
  client->abort();
  auto saddr = QUrl::fromUserInput(ipLineEdit->text());
  qDebug() << "host: " << saddr.host();
  qDebug() << "port: " << saddr.port();
  client->connectToHost(saddr.host(), saddr.port());
}

void Setup::finishClientSetup() {
  messageMan->setPeer(client);
  infoLabel->setText(tr("Connected!"));
  using namespace std::chrono_literals;
  QTimer::singleShot(2s, this, &Setup::close);
}

void Setup::displayError(QAbstractSocket::SocketError socketError) {
  switch (socketError) {
  case QAbstractSocket::RemoteHostClosedError:
    QMessageBox::information(this, tr("SIV Client"),
                             tr("Remote host closed before finished setup"));
    break;
  case QAbstractSocket::HostNotFoundError:
    QMessageBox::information(this, tr("SIV Client"),
                             tr("The host was not found. Please check the "
                                "host name and port settings."));
    break;
  case QAbstractSocket::ConnectionRefusedError:
    QMessageBox::information(this, tr("SIV Client"),
                             tr("The connection was refused by the peer. "
                                "Make sure the SIV server is running, "
                                "and check that the host name and port "
                                "settings are correct."));
    break;
  default:
    QMessageBox::information(this, tr("SIV Client"),
                             tr("The following error occurred: %1.").arg(client->errorString()));
  }
}

void Setup::serverSetup() {
  qDebug() << "server mode";
  server = new QTcpServer(messageMan);
  if (!server->listen()) {
    QMessageBox::critical(this, tr("Synchronized Image Viewer"),
                          tr("Unable to start the server: %1.").arg(server->errorString()));
    delete server;
    close();
    return;
  }
  auto ipAddress = QString{};
  // use the first non-localhost IPv4 address
  for (auto const& entry : QNetworkInterface::allAddresses()) {
    if (entry != QHostAddress::LocalHost && entry.toIPv4Address()) {
      ipAddress = entry.toString();
      break;
    }
  }
  // if we did not find one, use IPv4 localhost
  if (ipAddress.isEmpty()) {
    ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
  }

  infoLabel = new QLabel{};
  infoLabel->setMinimumSize(200, 70);
  layout()->addWidget(infoLabel);
  infoLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
  infoLabel->setText(tr("server is up\n\n%1:%2\n\n").arg(ipAddress).arg(server->serverPort()));

  connect(server, &QTcpServer::newConnection, this, &Setup::onNewConnection);
}

void Setup::onNewConnection() {
  server->pauseAccepting();
  auto peer = server->nextPendingConnection();
  connect(peer, &QTcpSocket::disconnected, peer, &QObject::deleteLater);
  messageMan->setPeer(peer);
  infoLabel->setText("Connected!");
  using namespace std::chrono_literals;
  QTimer::singleShot(2s, this, &Setup::close);
}
