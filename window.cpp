#include "window.h"

#include "setup.h"
#include "view_area.h"

#include <QFileDialog>
#include <QGuiApplication>
#include <QImageReader>
#include <QLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QStandardPaths>
#include <QTcpServer>

namespace {
void initializeImageOpenFileDialog(QFileDialog& dialog) {
  static auto firstDialog = true;

  if (firstDialog) {
    firstDialog = false;
    auto picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath()
                                                    : picturesLocations.last());
  }

  auto mimeTypeFilters = QStringList{};
  for (const QByteArray& mimeTypeName : QImageReader::supportedMimeTypes()) {
    mimeTypeFilters.append(mimeTypeName);
  }
  mimeTypeFilters.sort();
  dialog.setMimeTypeFilters(mimeTypeFilters);
  dialog.selectMimeTypeFilter("image/jpeg");
}
} // namespace

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
  auto fileMenu = menuBar()->addMenu(tr("&File"));
  auto openAct = fileMenu->addAction(tr("&Open"), this, &Window::openImage);
  openAct->setShortcut(QKeySequence::Open);
  setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
  show();
}

void Window::openImage() {
  QFileDialog dialog(this, tr("Open file"));
  initializeImageOpenFileDialog(dialog);
  while (dialog.exec() == QDialog::Accepted && !loadImage(dialog.selectedFiles().first())) {}
}

bool Window::loadImage(QString const& fileName) {
  auto reader = QImageReader(fileName);
  reader.setAutoTransform(true);
  auto image = reader.read();
  if (image.isNull()) {
    QMessageBox::information(
        this, QGuiApplication::applicationDisplayName(),
        tr("Cannot load %1: %2").arg(QDir::toNativeSeparators(fileName), reader.errorString()));
    return false;
  }
  mViewArea->setImage(image);
  adjustSize();
  setWindowFilePath(fileName);
  return true;
}
