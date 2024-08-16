#include "textinfobox.h"
#include "ui_textinfobox.h"
#include <QFontDialog>
#include <QObject>
#include <opencv2/core/types.hpp>
#include <qevent.h>
#include <qfont.h>
#include <qglobal.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qpalette.h>
#include <qtmetamacros.h>
#include <sstream>

TextInfoBox::TextInfoBox(textInf *data, QWidget *parent)
    : QWidget(parent), ui(new Ui::TextInfoBox) {
  ui->setupUi(this);
  this->data = data;
  this->ui->label->setText(QString::fromUtf8(data->text));

  data_intern = *new QFont();

  this->setAutoFillBackground(true);
  normalColor = QPalette();
  selectedColor = QPalette();
  selectedColor.setColor(QPalette::Window, Qt::lightGray);

  isSelected = false;

  QObject::connect(ui->editFont_Button, SIGNAL(clicked()), this,
                   SLOT(updateFont()));
}

TextInfoBox::~TextInfoBox() {
  delete ui;
}

void TextInfoBox::setText(QString text) {
  this->ui->label->setText(text);
  this->data->text = text.toStdString();
}

textInf *TextInfoBox::getData() {
  return this->data;
}

void TextInfoBox::updateFont() {
  bool ok;
  QFont font = QFontDialog::getFont(&ok, data_intern, this);
  if (ok) {
    // the user clicked OK and font is set to the font the user selected
    data_intern = font;
    // update textInf
    data->fontDesc = font;

    emit updated(this);
  } else {
    // the user canceled the dialog; font is set to the initial
    // value, in this case Helvetica [Cronyx], 10
  }
}

void TextInfoBox::mousePressEvent(QMouseEvent *event) {

  if (event->button() == Qt::LeftButton) {
    if (!isSelected) {
      this->setPalette(this->selectedColor);
      isSelected = true;
      emit selected(this);
    } else {
      this->setPalette(this->normalColor);
      isSelected = false;
      emit deselected(this);
    }
  }

  // default handling
  QWidget::mousePressEvent(event);
}
