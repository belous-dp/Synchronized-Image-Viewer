#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

class ViewArea;

class Window : public QMainWindow {
  Q_OBJECT

public:
  explicit Window(QWidget* parent = nullptr);

private:
  ViewArea* mViewArea;
};

#endif // WINDOW_H
