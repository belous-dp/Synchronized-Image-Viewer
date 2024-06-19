#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

class Setup;
class ViewArea;

class Window : public QMainWindow {
  Q_OBJECT

public:
  explicit Window(QWidget* parent = nullptr);

private slots:
  void onSetupFinished();

private:
  Setup* setupPopup;
  ViewArea* mViewArea;
};

#endif // WINDOW_H
