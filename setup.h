#ifndef SETUP_H
#define SETUP_H

#include <QDialog>

class QRadioButton;

class Setup : public QDialog {
  Q_OBJECT

public:
  explicit Setup(QWidget* parent = nullptr);

signals:
  void setupFinished();

private slots:
  void onOptionChosen();

private:
  QRadioButton* clientButton;
  QRadioButton* serverButton;
};

#endif // SETUP_H
