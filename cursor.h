#ifndef CURSOR_H
#define CURSOR_H

#include <QWidget>

class Cursor : public QWidget {
  Q_OBJECT

public:
  explicit Cursor(QWidget* parent = nullptr);

private:
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  // void mouseReleaseEvent(QMouseEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

private:
  QPoint offset;
};

#endif // CURSOR_H
