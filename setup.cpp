#include "setup.h"
#include "message_man.h"
#include <QCloseEvent>
#include <QLabel>
#include <QMessageBox>
#include <QTimer>
#include <QNetworkInterface>
#include <QRadioButton>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVBoxLayout>
#include <iostream>

Setup::Setup(MessageMan* messageMan, QWidget* parent)
    : QDialog(parent),
      messageMan(messageMan),
      server(nullptr),
      infoLabel(nullptr) {
  clientButton = new QRadioButton(tr("&client mode"));
  serverButton = new QRadioButton(tr("&server mode"));
  connect(clientButton, &QRadioButton::clicked, this, &Setup::onOptionChosen);
  connect(serverButton, &QRadioButton::clicked, this, &Setup::onOptionChosen);
  auto layout = new QVBoxLayout{};
  layout->addWidget(clientButton);
  layout->addWidget(serverButton);
  setLayout(layout);
}

void Setup::reject() {
  close();
}

void Setup::closeEvent(QCloseEvent* event) {
  std::cout << "closed" << std::endl;
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
  std::cout << "client mode" << std::endl;
  close();
}

void Setup::serverSetup() {
  std::cout << "server mode" << std::endl;
  server = new QTcpServer(messageMan);
  if (!server->listen()) {
    QMessageBox::critical(this, tr("Synchronized Image Viewer"),
                          tr("Unable to start the server: %1.").arg(server->errorString()));
    delete server; // todo move to closeEvent()?
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

  // test
  QByteArray data;
  QDataStream out(&data, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_6_7);
  out << "hello from server!";
  messageMan->sendMessage(data);
}
