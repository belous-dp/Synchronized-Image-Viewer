#include "cursor.h"
#include "message_man.h"
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include <QXmlStreamWriter>

Cursor::Cursor(MessageMan* messageMan, QWidget* parent)
    : QWidget(parent),
      messageMan(messageMan),
      timer(new QTimer(this)) {
  connect(timer, &QTimer::timeout, this, &Cursor::updateNetworkInfo);
  using namespace std::chrono_literals;
  timer->start(50ms);
}

void Cursor::mousePressEvent(QMouseEvent* event) {
  offset = event->pos();
  timer->start();
  assert(timer->interval() == 50);
}

void Cursor::mouseMoveEvent(QMouseEvent* event) {
  if (event->buttons() & Qt::LeftButton) {
    this->move(mapToParent(event->pos() - offset)); // = move(pos() + delta)
  }
}

void Cursor::mouseReleaseEvent(QMouseEvent* event) {
  updateNetworkInfo();
  timer->stop();
  assert(timer->interval() == 50);
}

void Cursor::paintEvent(QPaintEvent* event) {
  auto painter = QPainter(this);
  painter.fillRect(0, 0, 30, 30, Qt::white);
}

void Cursor::updateNetworkInfo() {
  if (pos() == lastPos) {
    return;
  }
  lastPos = pos();
  messageMan->getStreamWriter().writeStartElement("cursor");
  messageMan->getStreamWriter().writeAttribute("x", QString::number(lastPos.x()));
  messageMan->getStreamWriter().writeAttribute("y", QString::number(lastPos.y()));
  messageMan->getStreamWriter().writeEndElement();
  messageMan->flushMessage();
}
