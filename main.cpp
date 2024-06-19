#include "window.h"

#include <QApplication>

int main(int argc, char* argv[]) {
  auto app = QApplication(argc, argv);

  auto window = Window();

  return app.exec();
}
