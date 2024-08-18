#include "textInformation.h"
#include "textinfobox.h"

textInformation::textInformation(std::string text, QPoint pos, QPoint delta,
                                 QFont font, cv::Scalar color, int frameStart,
                                 int frameEnd, TextInfoBox *uiElement) {
  this->text = text;
  this->pos = pos;
  this->delta = delta;
  this->fontDesc = font;
  this->color = color;
  this->frameStart = frameStart;
  this->frameEnd = frameEnd;
  this->uiElement = uiElement;
}

std::string textInformation::getText() {
  return this->text;
}

QPoint textInformation::getPosition() {
  return this->pos;
}

cv::Point2d textInformation::getPosition_as_cvPoint() {
  cv::Point2d point = cv::Point2d(this->pos.x() + this->delta.x(),
                                  this->pos.y() + this->delta.y());
  return point;
}

QPoint textInformation::getDelta() {
  return this->delta;
}
QFont textInformation::getFont() {
  return this->fontDesc;
}
cv::Scalar textInformation::getColor() {
  return this->color;
}
int textInformation::getFrameStart() {
  return this->frameStart;
}
int textInformation::getFrameEnd() {
  return this->frameEnd;
}
TextInfoBox *textInformation::getUiElement() {
  return this->uiElement;
}

void textInformation::setText(std::string new_text) {
  this->text = new_text;
}

void textInformation::setFont(QFont new_font) {
  this->fontDesc = new_font;
}

void textInformation::setDelta(QPoint new_delta) {
  this->delta = new_delta;
}

PangoFontDescription *textInformation::getFont_as_Pango() {
  // PangoFontDescription erstellen
  PangoFontDescription *pangoDesc = pango_font_description_new();

  // Schriftname setzen
  pango_font_description_set_family(
      pangoDesc, this->fontDesc.family().toUtf8().constData());

  // Schriftgröße in Pango-Einheiten setzen (1 Punkt = PANGO_SCALE
  // Pango-Einheiten)
  pango_font_description_set_size(pangoDesc,
                                  this->fontDesc.pointSize() * PANGO_SCALE);

  // Schriftgewicht setzen
  PangoWeight weight = PANGO_WEIGHT_NORMAL;
  if (this->fontDesc.weight() == QFont::Thin) {
    weight = PANGO_WEIGHT_THIN;
  } else if (this->fontDesc.weight() == QFont::ExtraLight) {
    weight = PANGO_WEIGHT_ULTRALIGHT;
  } else if (this->fontDesc.weight() == QFont::Light) {
    weight = PANGO_WEIGHT_LIGHT;
  } else if (this->fontDesc.weight() == QFont::Normal) {
    weight = PANGO_WEIGHT_NORMAL;
  } else if (this->fontDesc.weight() == QFont::Medium) {
    weight = PANGO_WEIGHT_MEDIUM;
  } else if (this->fontDesc.weight() == QFont::DemiBold) {
    weight = PANGO_WEIGHT_SEMIBOLD;
  } else if (this->fontDesc.weight() == QFont::Bold) {
    weight = PANGO_WEIGHT_BOLD;
  } else if (this->fontDesc.weight() == QFont::ExtraBold) {
    weight = PANGO_WEIGHT_ULTRABOLD;
  } else if (this->fontDesc.weight() == QFont::Black) {
    weight = PANGO_WEIGHT_HEAVY;
  }
  pango_font_description_set_weight(pangoDesc, weight);

  // Schriftstil (kursiv) setzen
  PangoStyle style =
      this->fontDesc.italic() ? PANGO_STYLE_ITALIC : PANGO_STYLE_NORMAL;
  pango_font_description_set_style(pangoDesc, style);

  return pangoDesc;
}

void textInformation::applyDelta() {
  this->pos = this->pos + this->delta;
  this->delta.setX(0);
  this->delta.setY(0);
}
