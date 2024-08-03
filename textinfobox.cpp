#include "textinfobox.h"
#include "ui_textinfobox.h"
#include <QFontDialog>
#include <QObject>

TextInfoBox::TextInfoBox(QWidget *parent)
    : QWidget(parent), ui(new Ui::TextInfoBox) {
  ui->setupUi(this);

  QObject::connect(ui->editFont_Button, SIGNAL(clicked()), this,
                   SLOT(updateFont()));
}

TextInfoBox::~TextInfoBox() {
  delete ui;
}

void TextInfoBox::updateFont() {
  bool ok;
  QFont font = QFontDialog::getFont(&ok, *data_intern, this);
  if (ok) {
    // the user clicked OK and font is set to the font the user selected
  } else {
    // the user canceled the dialog; font is set to the initial
    // value, in this case Helvetica [Cronyx], 10
  }
}
