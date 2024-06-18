#include "cursor.h"
#include <QMouseEvent>
#include <QPainter>

Cursor::Cursor(QWidget* parent) : QWidget(parent) {}

void Cursor::mousePressEvent(QMouseEvent* event) {
  offset.setX(event->pos().x());
  offset.setY(event->pos().y());
}

void Cursor::mouseMoveEvent(QMouseEvent* event) {
  if (event->buttons() & Qt::LeftButton) {
    this->move(mapToParent(event->pos() - offset));
  }
}

void Cursor::paintEvent(QPaintEvent* event) {
  auto painter = QPainter(this);
  painter.fillRect(0, 0, 30, 30, Qt::white);
}
