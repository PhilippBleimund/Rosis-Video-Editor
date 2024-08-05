#pragma once

#include "textinfobox.h"
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>
#include <qimage.h>
#include <string>
#include <vector>

#include <QVideoWidget>
#include <qobjectdefs.h>
#include <qtmetamacros.h>

class videoObj : public cv::VideoCapture {

public:
  void setFPS(int);
  void setNumFrames(int);
  int getFPS();
  int getNumFrames();
  QImage getImage();
  int addText(std::string);
  textInf *getText(int);
  virtual bool open(const cv::String &);

  bool updateFrame(); // slot for updating the current frame
  void repaintFrame();

private:
  int fps;
  int numFrames;
  int currFrame;
  std::vector<textInf> textList;
  cv::Mat frame;
  QImage qtImage;
};
