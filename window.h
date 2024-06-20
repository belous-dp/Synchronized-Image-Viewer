#ifndef WINDOW_H
#define WINDOW_H

#include "message_man.h"
#include <QMainWindow>

class Setup;
class ViewArea;

class Window : public QMainWindow {
  Q_OBJECT

public:
  explicit Window(QWidget* parent = nullptr);

private slots:
  void onSetupFinished();
  void openImage();

private:
  bool loadImage(QString const& fileName);

private:
  MessageMan messageMan;
  Setup* setupPopup;
  ViewArea* mViewArea;
};

#endif // WINDOW_H
