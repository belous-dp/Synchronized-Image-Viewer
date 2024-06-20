#include "view_area.h"
#include <QLabel>

ViewArea::ViewArea(MessageMan* messageMan, QWidget* parent)
    : QWidget(parent),
      cursor(messageMan, this),
      imageLabel(new QLabel(this)) {
  cursor.setEnabled(false);
  // imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  // imageLabel->setScaledContents(true);
  imageLabel->setEnabled(false);
  // setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}

void ViewArea::setImage(QImage const& image) {
  // todo color space and color depth
  imageLabel->setPixmap(QPixmap::fromImage(image));
  imageLabel->adjustSize();
  imageLabel->setEnabled(true);
  cursor.setEnabled(true);
  cursor.raise();
  adjustSize();
}
