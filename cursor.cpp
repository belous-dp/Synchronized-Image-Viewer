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
  connect(messageMan, &MessageMan::messageReady, this, &Cursor::updateCursorPos);
  connect(timer, &QTimer::timeout, this, &Cursor::updateNetworkInfo);
  using namespace std::chrono_literals;
  timer->start(50ms);
}

void Cursor::mousePressEvent(QMouseEvent* event) {
  offset = event->pos();
  timer->start();
}

void Cursor::mouseMoveEvent(QMouseEvent* event) {
  if (event->buttons() & Qt::LeftButton) {
    this->move(mapToParent(event->pos() - offset)); // = move(pos() + delta)
  }
}

void Cursor::mouseReleaseEvent(QMouseEvent* event) {
  updateNetworkInfo();
  timer->stop();
}

void Cursor::paintEvent(QPaintEvent* event) {
  auto painter = QPainter(this);
  painter.fillRect(13, 0, 4, 30, Qt::white);
  painter.fillRect(0, 13, 30, 4, Qt::white);
  painter.fillRect(14, 1, 2, 28, Qt::black);
  painter.fillRect(1, 14, 28, 2, Qt::black);
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
  messageMan->finishWrite();
}

void Cursor::updateCursorPos() {
  auto& reader = messageMan->getStreamReader();
  while (!reader.atEnd() && reader.readNextStartElement() && reader.name() != tr("cursor")) {
    reader.skipCurrentElement();
  }
  assert(reader.isStartElement() && reader.name() == tr("cursor"));
  assert(reader.attributes().hasAttribute("x"));
  assert(reader.attributes().hasAttribute("y"));
  qDebug() << "received x=" << reader.attributes().value("x").toInt()
           << " y=" << reader.attributes().value("y").toInt();
  move(reader.attributes().value("x").toInt(), reader.attributes().value("y").toInt());
  messageMan->finishRead();
}
