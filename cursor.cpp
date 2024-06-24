#include "cursor.h"
#include "message_man.h"
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include <QXmlStreamWriter>

Cursor::Cursor(MessageMan const& messageMan, QWidget* parent)
    : QWidget(parent),
      messageManClient(&messageMan) {
  // asserts needed in paintEvent
  static_assert(INNER + OUTER * 2 <= SIDE);
  static_assert(SIDE % 2 == 0);
  static_assert(INNER % 2 == 0);
  static_assert(INNER + OUTER > 0);
  workArea = parent ? parent->rect() : rect();
  connect(&messageMan, &MessageMan::messageReady, this, &Cursor::updateCursorPos);
  connect(&timer, &QTimer::timeout, this, &Cursor::updateNetworkInfo);
  using namespace std::chrono_literals;
  timer.setInterval(25ms);
}
void Cursor::setWorkAreaSize(QSize const& newWorkAreaSize) {
  if (!newWorkAreaSize.isValid()) {
    throw std::invalid_argument("Cursor: invalid newWorkAreaSize");
  }
  // todo newWorkAreaSize < (SIDE, SIDE)?
  workArea = QRect(QPoint(0, 0), newWorkAreaSize);
}

void Cursor::mousePressEvent(QMouseEvent* event) {
  offset = event->pos();
  timer.start();
}

void Cursor::mouseMoveEvent(QMouseEvent* event) {
  if (event->buttons() & Qt::LeftButton) {
    auto newPos = mapToParent(event->pos() - offset); // = pos() + delta
    if (workArea.contains(newPos.x() + SIDE / 2, newPos.y() + SIDE / 2)) {
      this->move(newPos);
    }
  }
}

void Cursor::mouseReleaseEvent(QMouseEvent* event) {
  updateNetworkInfo();
  timer.stop();
}

void Cursor::paintEvent(QPaintEvent* event) {
  auto painter = QPainter(this);
  painter.fillRect(SIDE / 2 - INNER / 2 - OUTER, 0, INNER + 2 * OUTER, SIDE, Qt::white);
  painter.fillRect(0, SIDE / 2 - INNER / 2 - OUTER, SIDE, INNER + 2 * OUTER, Qt::white);
  painter.fillRect(SIDE / 2 - INNER / 2, OUTER, INNER, SIDE - 2 * OUTER, Qt::black);
  painter.fillRect(OUTER, SIDE / 2 - INNER / 2, SIDE - 2 * OUTER, INNER, Qt::black);
}

void Cursor::updateNetworkInfo() {
  if (pos() == lastPos) {
    return;
  }
  lastPos = pos();
  messageManClient.getStreamWriter().writeStartElement("cursor");
  messageManClient.getStreamWriter().writeAttribute("x", QString::number(lastPos.x()));
  messageManClient.getStreamWriter().writeAttribute("y", QString::number(lastPos.y()));
  messageManClient.getStreamWriter().writeEndElement();
  messageManClient.finishWrite();
}

void Cursor::updateCursorPos() {
  // timer->stop(); // todo synchronization issues
  auto& reader = messageManClient.getStreamReader();
  while (!reader.atEnd() && messageManClient.xmlReadNextStartElement() &&
         reader.name() != tr("cursor")) {
    messageManClient.xmlSkipCurrentElement();
  }
  assert(reader.isStartElement() && reader.name() == tr("cursor"));
  assert(reader.attributes().hasAttribute("x"));
  assert(reader.attributes().hasAttribute("y"));
  move(reader.attributes().value("x").toInt(), reader.attributes().value("y").toInt());
  messageManClient.finishRead();
}
