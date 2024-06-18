#include "view_area.h"
#include "window.h"

#include <QApplication>
#include <QCommandLineParser>
#include <iostream>

void server_setup() {
  std::cout << "server mode" << std::endl;
}

void client_setup(QString const& server_ip) {
  std::cout << "client mode" << std::endl;
  std::cout << qPrintable(server_ip) << std::endl;
}

int main(int argc, char* argv[]) {
  auto app = QApplication(argc, argv);
  auto parser = QCommandLineParser();
  parser.addPositionalArgument("ip", "Address of a server");
  parser.process(app);

  if (parser.positionalArguments().isEmpty()) {
    server_setup();
  } else {
    auto server_ip = parser.positionalArguments()[0];
    client_setup(server_ip);
  }

  auto window = Window();
  window.show();

  return app.exec();
}
