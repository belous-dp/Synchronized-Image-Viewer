#ifndef VIEW_AREA_H
#define VIEW_AREA_H

#include "cursor.h"

#include <QWidget>

class ViewArea : public QWidget {
  Q_OBJECT

public:
  explicit ViewArea(QWidget* parent = nullptr);

private:
  Cursor mCursor;
};

#endif // VIEW_AREA_H
