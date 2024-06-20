#ifndef VIEW_AREA_H
#define VIEW_AREA_H

#include "cursor.h"

#include <QWidget>

class QLabel;

class ViewArea : public QWidget {
  Q_OBJECT

public:
  explicit ViewArea(QWidget* parent = nullptr);
  void setImage(QImage const& image);

private:
  Cursor mCursor;
  QLabel* imageLabel;
};

#endif // VIEW_AREA_H
