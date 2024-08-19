#include "textInformation.h"
#include "textinfobox.h"
#include <algorithm>
#include <memory>

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

textInformation::textInformation(const textInformation *a) {

  this->text = a->text;
  this->pos = a->pos;
  this->delta = a->delta;
  this->fontDesc = a->fontDesc;
  this->color = a->color;
  this->frameStart = a->frameStart;
  this->frameEnd = a->frameEnd;
  this->uiElement = a->uiElement;
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

void textInformation::goToPast() {
  // create copy of this
  std::unique_ptr<textInformation> copy_uniq(new textInformation(*this));
  // add copy to future
  this->future->push_back(std::move(copy_uniq));

  // get past
  textInformation *old_element = this->past->back().get();

  // apply changes
  this->text = old_element->text;
  this->pos = old_element->pos;
  this->delta = old_element->delta;
  this->fontDesc = old_element->fontDesc;
  this->color = old_element->color;
  this->frameStart = old_element->frameStart;
  this->frameEnd = old_element->frameEnd;

  // remove object from past
  this->past->pop_back();
}

void textInformation::goToFuture() {
  // create copy of this
  std::unique_ptr<textInformation> copy_uniq(new textInformation(*this));
  // add copy to past
  this->past->push_back(std::move(copy_uniq));

  // get futue
  textInformation *new_element = this->future->back().get();

  // apply changes
  this->text = new_element->text;
  this->pos = new_element->pos;
  this->delta = new_element->delta;
  this->fontDesc = new_element->fontDesc;
  this->color = new_element->color;
  this->frameStart = new_element->frameStart;
  this->frameEnd = new_element->frameEnd;

  // remove object from past
  this->future->pop_back();
}

void textInformation::createPast() {
  // create copy of this
  std::unique_ptr<textInformation> copy_uniq(new textInformation(*this));
  // check if MAX_SAVES is exceeded
  if (this->past->size() >= MAX_SAVES) {
    this->past->erase(this->past->begin());
  }
  // add copy to past
  this->past->push_back(std::move(copy_uniq));
}

void textInformation::createFuture() {
  // create copy of this
  std::unique_ptr<textInformation> copy_uniq(new textInformation(*this));
  // check if MAX_SAVES is exceeded
  if (this->future->size() >= MAX_SAVES) {
    this->future->erase(this->future->begin());
  }
  // add copy to past
  this->future->push_back(std::move(copy_uniq));
}
