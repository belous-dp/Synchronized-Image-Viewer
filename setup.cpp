#include "setup.h"
#include <QRadioButton>
#include <QVBoxLayout>

Setup::Setup(QWidget* parent) : QDialog(parent) {
  clientButton = new QRadioButton(tr("&client mode"));
  serverButton = new QRadioButton(tr("&server mode"));
  connect(clientButton, &QRadioButton::clicked, this, &Setup::onOptionChosen);
  connect(serverButton, &QRadioButton::clicked, this, &Setup::onOptionChosen);
  auto layout = new QVBoxLayout{};
  layout->addWidget(clientButton);
  layout->addWidget(serverButton);
  setLayout(layout);
}

void Setup::onOptionChosen() {
  emit setupFinished();
  close();
}
