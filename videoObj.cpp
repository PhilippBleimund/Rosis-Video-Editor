#include "videoObj.h"
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/videoio.hpp>

#include <QDebug>
#include <qimage.h>
#include <qvideowidget.h>
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

void videoObj::addText(std::string text) {
  textInf newElement;
  newElement.text = text;
  newElement.color = cv::Scalar(0, 0, 0, 0);
  newElement.thickness = 3;
  newElement.fontScale = 3;
  newElement.y_pos = 150;
  newElement.x_pos = 50;
  newElement.frameEnd = 250;
  newElement.frameStart = 0;

  this->textList.push_back(newElement);
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

// Function to overlay text onto the video
void overlayText(cv::Mat &frame, const std::string &text, int x, int y,
                 double fontScale, int thickness, cv::Scalar color) {

  // split text in lines
  std::vector<std::string> lines;
  std::stringstream ss(text);
  std::string item;

  while (getline(ss, item, '\n')) {
    lines.push_back(item);
  }

  // add text line for line start at bottom
  for (int i = lines.size() - 1; i >= 0; i--) {
    int baseline = 0;
    cv::Size textSize = cv::getTextSize(lines[i], cv::FONT_HERSHEY_SIMPLEX,
                                        fontScale, thickness, &baseline);
    // calculate new position for line
    y = y + textSize.height;
    cv::putText(frame, lines[i], cv::Point(x, y), cv::FONT_HERSHEY_SIMPLEX,
                fontScale, color, thickness);
  }
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
      overlayText(frame, textObj.text, textObj.x_pos, textObj.y_pos,
                  textObj.fontScale, textObj.thickness, textObj.color);
    }
  }

  // Convert the frame to a QImage
  qtImage = QImage(frame.data, frame.cols, frame.rows, frame.step,
                   QImage::Format_RGB888);

  currFrame++;
  return true;
}
