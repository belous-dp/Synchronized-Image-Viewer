#ifndef VIEW_AREA_H
#define VIEW_AREA_H

#include "cursor.h"

#include <QScrollArea>

class QLabel;

class ViewArea : public QScrollArea {
  Q_OBJECT

public:
  explicit ViewArea(QWidget* parent = nullptr);
  void setImage(QImage const& image);

private:
  Cursor mCursor;
  QLabel* imageLabel;
};

#endif // VIEW_AREA_H
