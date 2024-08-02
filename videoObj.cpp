#include "videoObj.h"
#include <opencv2/core/cvstd.hpp>
#include <opencv2/videoio.hpp>

#include <QDebug>
#include <qimage.h>
#include <qvideowidget.h>

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

bool videoObj::open(const cv::String &filename) {
  bool status = cv::VideoCapture::open(filename);

  if (status == false)
    return status;

  this->setFPS(static_cast<int>(this->get(cv::CAP_PROP_FPS)));
  this->setNumFrames(static_cast<int>(this->get(cv::CAP_PROP_FRAME_COUNT)));

  return status;
}

bool videoObj::updateFrame() {
  *this >> frame;
  if (frame.empty()) {
    qDebug() << "End of video.";
    return false;
  }

  // Convert the frame to a QImage
  qtImage = QImage(frame.data, frame.cols, frame.rows, frame.step,
                   QImage::Format_RGB888);

  return true;
}
