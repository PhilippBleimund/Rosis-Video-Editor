#include "textinfobox.h"
#include "textInformation.h"
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

TextInfoBox::TextInfoBox(textInformation *data, QWidget *parent)
    : QWidget(parent), ui(new Ui::TextInfoBox) {
  ui->setupUi(this);
  this->data = data;
  this->ui->label->setText(QString::fromUtf8(data->getText()));

  this->setAutoFillBackground(true);
  normalColor = QPalette();
  selectedColor = QPalette();
  selectedColor.setColor(QPalette::Window, Qt::lightGray);

  isSelected = false;

  QObject::connect(ui->editFont_Button, SIGNAL(clicked()), this,
                   SLOT(updateFont()));

  QObject::connect(ui->delete_Button, SIGNAL(clicked()), this,
                   SLOT(deleteThis()));
}

TextInfoBox::~TextInfoBox() {
  delete ui;
}

void TextInfoBox::setText(QString text) {
  this->ui->label->setText(text);
  this->data->setText(text.toStdString());
}

textInformation *TextInfoBox::getData() {
  return this->data;
}

void TextInfoBox::setData(textInformation *newData) {
  this->data = newData;
}

void TextInfoBox::updateFont() {
  bool ok;
  QFont font = QFontDialog::getFont(&ok, data->getFont(), this);
  if (ok) {
    // the user clicked OK and font is set to the font the user selected

    emit updated(this, font);
  } else {
    // the user canceled the dialog; font is set to the initial
    // value, in this case Helvetica [Cronyx], 10
  }
}

void TextInfoBox::deleteThis() {
  emit deleted(this);
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
