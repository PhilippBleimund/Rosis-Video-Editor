#ifndef TEXTINFOBOX_H
#define TEXTINFOBOX_H

#include <QWidget>
#include <opencv2/opencv.hpp>
#include <qevent.h>
#include <qfont.h>
#include <qobject.h>
#include <qpalette.h>
#include <string>

namespace Ui {
class TextInfoBox;
}

struct textInf {
  std::string text;
  int x_pos;
  int y_pos;
  int x_delta;
  int y_delta;
  QFont fontDesc;
  cv::Scalar color;
  int frameStart;
  int frameEnd;
  Ui::TextInfoBox *uiElement;
};

class TextInfoBox : public QWidget {
  Q_OBJECT

public:
  explicit TextInfoBox(textInf *, QWidget *parent = nullptr);
  ~TextInfoBox();
  void setText(QString);
  textInf *getData();

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
  textInf *data;
  QFont data_intern;
  QPalette normalColor;
  QPalette selectedColor;
  bool isSelected;
};

#endif // TEXTINFOBOX_H
