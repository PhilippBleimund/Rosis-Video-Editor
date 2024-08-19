#ifndef TEXTINFOBOX_H
#define TEXTINFOBOX_H

#include <QWidget>
#include <opencv2/opencv.hpp>
#include <qevent.h>
#include <qfont.h>
#include <qobject.h>
#include <qpalette.h>

namespace Ui {
class TextInfoBox;
}

class textInformation;

class TextInfoBox : public QWidget {
  Q_OBJECT

public:
  explicit TextInfoBox(textInformation *, QWidget *parent = nullptr);
  ~TextInfoBox();
  void setText(QString);
  textInformation *getData();
  void setData(textInformation *);

public slots:
  void updateFont();

signals:
  void updated(TextInfoBox *);
  void selected(TextInfoBox *);
  void deselected(TextInfoBox *);

protected:
  void mousePressEvent(QMouseEvent *);

private:
  Ui::TextInfoBox *ui;
  textInformation *data;
  QFont data_intern;
  QPalette normalColor;
  QPalette selectedColor;
  bool isSelected;
};

#endif // TEXTINFOBOX_H
