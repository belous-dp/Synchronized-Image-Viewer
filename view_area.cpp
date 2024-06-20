#include "view_area.h"
#include <QLabel>

ViewArea::ViewArea(QWidget* parent) : QScrollArea(parent), mCursor(this), imageLabel(new QLabel{}) {
  imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  imageLabel->setScaledContents(true);
  setWidget(imageLabel);
  mCursor.setEnabled(false);
}

void ViewArea::setImage(QImage const& image) {
  // todo color space
  imageLabel->setPixmap(QPixmap::fromImage(image));
  imageLabel->adjustSize();
  mCursor.setEnabled(true);
  mCursor.raise();
}
