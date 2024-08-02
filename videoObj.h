#pragma once

#include <opencv2/core/cvstd.hpp>
#include <opencv2/opencv.hpp>
#include <qimage.h>
#include <vector>

#include <QVideoWidget>
#include <qobjectdefs.h>
#include <qtmetamacros.h>

struct textInf {};

class videoObj : public cv::VideoCapture {

public:
  void setFPS(int);
  void setNumFrames(int);
  int getFPS();
  int getNumFrames();
  QImage getImage();
  virtual bool open(const cv::String &);

  bool updateFrame(); // slot for updating the current frame

private:
  int fps;
  int numFrames;
  std::vector<textInf> textList;
  cv::Mat frame;
  QImage qtImage;
};
