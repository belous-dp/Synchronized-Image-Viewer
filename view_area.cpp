#include "view_area.h"
#include <QLabel>

ViewArea::ViewArea(QWidget* parent) : QWidget(parent), mCursor(this), imageLabel(new QLabel(this)) {
  mCursor.setEnabled(false);
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
  mCursor.setEnabled(true);
  mCursor.raise();
  adjustSize();
}
