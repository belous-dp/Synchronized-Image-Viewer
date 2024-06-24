#ifndef CURSOR_H
#define CURSOR_H

#include "client.h"

#include <QTimer>
#include <QWidget>

class Cursor : public QWidget {
  Q_OBJECT

public:
  explicit Cursor(MessageMan const& messageMan, QWidget* parent = nullptr);
  void setWorkAreaSize(QSize const& newWorkAreaSize);

private:
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

private slots:
  void updateNetworkInfo();
  void updateCursorPos();

private:
  static constexpr int SIDE = 30;
  static constexpr int INNER = 2;
  static constexpr int OUTER = 1;
  QRect workArea;
  QPoint offset;
  QPoint lastPos;
  Client messageManClient;
  QTimer timer;
};

#endif // CURSOR_H
