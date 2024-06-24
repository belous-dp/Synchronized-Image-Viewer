#ifndef VIEW_AREA_H
#define VIEW_AREA_H

#include "cursor.h"

#include <QWidget>

class MessageMan;
class QLabel;

class ViewArea : public QWidget {
  Q_OBJECT

public:
  explicit ViewArea(MessageMan const& messageMan, QWidget* parent = nullptr);
  void setImage(QImage const& image);

private:
  Cursor cursor;
  QLabel* imageLabel;
};

#endif // VIEW_AREA_H
