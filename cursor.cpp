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
  // asserts needed in paintEvent
  static_assert(INNER + OUTER * 2 <= SIDE);
  static_assert(SIDE % 2 == 0);
  static_assert(INNER % 2 == 0);
  static_assert(INNER + OUTER > 0);
  workArea = parent ? parent->rect() : rect();
  connect(messageMan, &MessageMan::messageReady, this, &Cursor::updateCursorPos);
  connect(timer, &QTimer::timeout, this, &Cursor::updateNetworkInfo);
  using namespace std::chrono_literals;
  timer->setInterval(25ms);
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
  timer->start();
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
  timer->stop();
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
  messageMan->getStreamWriter().writeStartElement("cursor");
  messageMan->getStreamWriter().writeAttribute("x", QString::number(lastPos.x()));
  messageMan->getStreamWriter().writeAttribute("y", QString::number(lastPos.y()));
  messageMan->getStreamWriter().writeEndElement();
  messageMan->finishWrite();
}

void Cursor::updateCursorPos() {
  // timer->stop(); // todo synchronization issues
  auto& reader = messageMan->getStreamReader();
  while (!reader.atEnd() && reader.readNextStartElement() && reader.name() != tr("cursor")) {
    reader.skipCurrentElement();
  }
  assert(reader.isStartElement() && reader.name() == tr("cursor"));
  assert(reader.attributes().hasAttribute("x"));
  assert(reader.attributes().hasAttribute("y"));
  move(reader.attributes().value("x").toInt(), reader.attributes().value("y").toInt());
  messageMan->finishRead();
}
