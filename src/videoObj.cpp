#include "videoObj.h"
#include "glib-object.h"
#include "pango/pango-font.h"
#include "textInformation.h"
#include <memory>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/videoio.hpp>
#include <pango/pangocairo.h>

#include <QDebug>
#include <qfont.h>
#include <qglobal.h>
#include <qimage.h>
#include <qvideowidget.h>
#include <string>
#include <vector>

void videoObj::setFPS(int f) {
  this->fps = f;
}

void videoObj::setNumFrames(int f) {
  this->numFrames = f;
}

int videoObj::getFPS() {
  return this->fps;
}

int videoObj::getNumFrames() {
  return this->numFrames;
}

QImage videoObj::getImage() {
  return this->qtImage;
}

int videoObj::addText(std::string text) {
  std::unique_ptr<textInformation> newElement =
      std::make_unique<textInformation>(text, QPoint(0, 0), QPoint(0, 0),
                                        QFont(), cv::Scalar(0, 0, 0), 0, 250);
  this->textList.push_back(std::move(newElement));
  return this->textList.back()->getUid();
}

textInformation *videoObj::getText(int uid) {
  for (std::vector<std::unique_ptr<textInformation>>::iterator i =
           this->textList.begin();
       i != this->textList.end(); i++) {
    if (i->get()->getUid() == uid) {
      return i->get();
    }
  }
  return nullptr;
}

bool videoObj::open(const cv::String &filename) {
  bool status = cv::VideoCapture::open(filename);

  if (status == false)
    return status;

  this->setFPS(static_cast<int>(this->get(cv::CAP_PROP_FPS)));
  this->setNumFrames(static_cast<int>(this->get(cv::CAP_PROP_FRAME_COUNT)));
  this->currFrame = 0;

  return status;
}

void putTextCairo(cv::Mat &targetImage, std::string text,
                  cv::Point2d centerPoint, PangoFontDescription *pango_desc,
                  cv::Scalar textColor) {

  // Create Cairo
  cairo_surface_t *surface = cairo_image_surface_create(
      CAIRO_FORMAT_ARGB32, targetImage.cols, targetImage.rows);

  cairo_t *cairo = cairo_create(surface);

  // Create pango
  PangoLayout *layout;

  /* Create a PangoLayout, set the font and text */

  layout = pango_cairo_create_layout(cairo);
  pango_layout_set_text(layout, text.c_str(), -1);
  pango_layout_set_font_description(layout, pango_desc);
  pango_font_description_free(pango_desc);

  // Wrap Cairo with a Mat
  cv::Mat cairoTarget(cairo_image_surface_get_height(surface),
                      cairo_image_surface_get_width(surface), CV_8UC4,
                      cairo_image_surface_get_data(surface),
                      cairo_image_surface_get_stride(surface));

  // Put image onto Cairo
  cv::cvtColor(targetImage, cairoTarget, cv::COLOR_BGR2BGRA);

  cairo_set_source_rgb(cairo, textColor[2], textColor[1], textColor[0]);

  // set position
  cairo_move_to(cairo, centerPoint.x, centerPoint.y);

  // Put Text onto image
  pango_cairo_show_layout(cairo, layout);

  // Copy the data to the output image
  cv::cvtColor(cairoTarget, targetImage, cv::COLOR_BGRA2BGR);

  g_object_unref(layout);
  cairo_destroy(cairo);
  cairo_surface_destroy(surface);
}

bool videoObj::updateFrame() {
  *this >> frame;
  if (frame.empty()) {
    qDebug() << "End of video.";
    setToStart();
    return false;
  }

  // check if text is available
  for (std::unique_ptr<textInformation> &textObj : this->textList) {
    if (currFrame >= textObj->getFrameStart() &&
        currFrame <= textObj->getFrameEnd()) {
      // text can be printed
      putTextCairo(frame, textObj->getText(), textObj->getPosition_as_cvPoint(),
                   textObj->getFont_as_Pango(), textObj->getColor());
    }
  }

  // Convert the frame to a QImage
  qtImage = QImage(frame.data, frame.cols, frame.rows, frame.step,
                   QImage::Format_RGB888);

  currFrame++;
  return true;
}

void videoObj::repaintFrame() {
  this->set(cv::CAP_PROP_POS_FRAMES, --currFrame);
  updateFrame();
}

void videoObj::setToStart() {
  this->set(cv::CAP_PROP_POS_FRAMES, 0);
  currFrame = 0;
  updateFrame();
}

void videoObj::goToPast() {
  if (!this->past.size()) {
    return;
  }
  // get last modified state
  std::vector<std::unique_ptr<textInformation>>::iterator latest =
      --this->past.end();

  // get element with corresponding uid
  std::vector<std::unique_ptr<textInformation>>::iterator element;
  for (auto i = std::begin(this->textList); i != std::end(this->textList);
       ++i) {
    if ((*i)->getUid() == (*latest)->getUid()) {
      element = i;
    }
  }

  // move element to future
  this->future.push_back(std::move(*element));
  this->textList.erase(element);

  //// replace present with past
  (*latest)->getUiElement()->setData((*latest).get());
  this->textList.push_back(std::move(*latest));
  this->past.erase(latest);
}

void videoObj::goToFuture() {
  if (!this->future.size()) {
    return;
  }
  // get first future state
  std::vector<std::unique_ptr<textInformation>>::iterator latest =
      --this->future.end();

  // get element with corresponding uid
  std::vector<std::unique_ptr<textInformation>>::iterator element;
  for (auto i = std::begin(this->textList); i != std::end(this->textList);
       ++i) {
    if ((*i)->getUid() == (*latest)->getUid()) {
      element = i;
    }
  }

  // move element to past
  this->past.push_back(std::move(*element));
  this->textList.erase(element);

  // replace present with future
  (*latest)->getUiElement()->setData((*latest).get());
  this->textList.push_back(std::move(*latest));
  this->future.erase(latest);
}

void videoObj::createPast(int uid) {
  // remove oldest element
  if (this->past.size() >= MAX_SAVES) {
    this->past.erase(this->past.begin());
  }

  // find element to back up
  std::unique_ptr<textInformation> *element;
  for (std::unique_ptr<textInformation> &item : this->textList) {
    if (item->getUid() == uid) {
      element = &item;
    }
  }

  // create copy
  std::unique_ptr<textInformation> copy =
      std::make_unique<textInformation>(element->get(), true);
  // put copy on past timeline
  this->past.push_back(std::move(copy));
  // clear Future since new timeline started
  clearFuture();
}

void videoObj::clearFuture() {
  this->future.clear();
}
