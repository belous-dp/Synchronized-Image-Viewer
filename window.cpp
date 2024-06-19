#include "window.h"

#include "setup.h"
#include "view_area.h"

#include <QLayout>
#include <QTcpServer>
#include <iostream>

Window::Window(QWidget* parent)
    : QMainWindow(parent),
      setupPopup(new Setup(this)),
      mViewArea(new ViewArea(this)) {
  setMinimumSize(300, 200);
  connect(setupPopup, &Setup::setupFinished, this, &Window::onSetupFinished);
  setCentralWidget(mViewArea);
  setWindowState(Qt::WindowMinimized);
  setupPopup->show();
}

void Window::onSetupFinished() {
  std::cout << "finished" << std::endl;
  setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
  show();
  delete setupPopup;
}
