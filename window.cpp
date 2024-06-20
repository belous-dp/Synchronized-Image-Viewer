#include "window.h"

#include "setup.h"
#include "view_area.h"

#include <QLayout>
#include <QTcpServer>

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
  qDebug() << "setup finished; now show main window...";
  setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
  show();
}
