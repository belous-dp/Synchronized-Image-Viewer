#include "window.h"

#include "view_area.h"

Window::Window(QWidget* parent) : QMainWindow(parent), mViewArea(new ViewArea(this)) {
  resize(size());
  setCentralWidget(mViewArea);
}
