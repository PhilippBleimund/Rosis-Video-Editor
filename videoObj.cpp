#include "videoObj.h"
#include "glib-object.h"
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/videoio.hpp>
#include <pango/pangocairo.h>

#include <QDebug>
#include <qimage.h>
#include <qvideowidget.h>
#include <sstream>
#include <string>

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
  textInf newElement;
  newElement.text = text;
  newElement.color = cv::Scalar(0, 0, 0, 0);
  newElement.fontScale = 100;
  newElement.y_pos = 350;
  newElement.x_pos = 450;
  newElement.frameEnd = 250;
  newElement.frameStart = 0;

  this->textList.push_back(newElement);
  return textList.size() - 1;
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

void putTextCairo(cv::Mat &targetImage, std::string &text,
                  cv::Point2d centerPoint, std::string &fontFace,
                  double fontSize, cv::Scalar textColor, bool fontItalic,
                  bool fontBold) {

  // Create Cairo
  cairo_surface_t *surface = cairo_image_surface_create(
      CAIRO_FORMAT_ARGB32, targetImage.cols, targetImage.rows);

  cairo_t *cairo = cairo_create(surface);

  // Create pango
  PangoLayout *layout;
  PangoFontDescription *desc;

  /* Create a PangoLayout, set the font and text */
  std::stringstream font_desc;
  font_desc << fontFace << " ";
  font_desc << fontSize << " ";

  layout = pango_cairo_create_layout(cairo);
  pango_layout_set_text(layout, text.c_str(), -1);
  desc = pango_font_description_from_string(font_desc.str().c_str());
  pango_layout_set_font_description(layout, desc);
  pango_font_description_free(desc);

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
    return false;
  }

  // check if text is available
  for (int i = 0; i < textList.size(); i++) {
    textInf textObj = textList[i];
    if (currFrame >= textObj.frameStart && currFrame <= textObj.frameEnd) {
      // text can be printed
      putTextCairo(frame, textObj.text,
                   cv::Point2d(textObj.x_pos, textObj.y_pos), textObj.fontFace,
                   textObj.fontScale, textObj.color, textObj.fontItalic,
                   textObj.fontBold);
    }
  }

  // Convert the frame to a QImage
  qtImage = QImage(frame.data, frame.cols, frame.rows, frame.step,
                   QImage::Format_RGB888);

  currFrame++;
  return true;
}
