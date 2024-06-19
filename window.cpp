#include "window.h"

#include "setup.h"
#include "view_area.h"

#include <QLayout>
#include <QTcpServer>
#include <iostream>

Window::Window(QWidget* parent)
    : QMainWindow(parent),
      setupPopup(new Setup(&messageMan, this)),
      mViewArea(new ViewArea(this)) {
  setMinimumSize(300, 200);
  setupPopup->setAttribute(Qt::WA_DeleteOnClose);
  connect(setupPopup, &Setup::setupFinished, this, &Window::onSetupFinished);
  setCentralWidget(mViewArea);
  setWindowState(Qt::WindowMinimized);
  setupPopup->show();
}

void Window::onSetupFinished() {
  std::cout << "setup finished; now show main window..." << std::endl;
  setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
  show();
}
