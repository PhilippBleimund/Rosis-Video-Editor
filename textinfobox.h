#ifndef TEXTINFOBOX_H
#define TEXTINFOBOX_H

#include "videoObj.h"
#include <QWidget>
#include <qfont.h>

namespace Ui {
class TextInfoBox;
}

class TextInfoBox : public QWidget {
  Q_OBJECT

public:
  explicit TextInfoBox(QWidget *parent = nullptr);
  ~TextInfoBox();

public slots:
  void updateFont();

private:
  Ui::TextInfoBox *ui;
  textInf *data;
  QFont *data_intern;
};

#endif // TEXTINFOBOX_H
