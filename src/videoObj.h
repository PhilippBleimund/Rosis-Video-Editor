#pragma once

#include "textInformation.h"
#include "textinfobox.h"
#include "timeline.h"
#include <memory>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>
#include <qimage.h>
#include <string>
#include <vector>

#include <QVideoWidget>
#include <qobjectdefs.h>
#include <qtmetamacros.h>

class videoObj : public cv::VideoCapture, timeline {

public:
  void setFPS(int);
  void setNumFrames(int);
  int getFPS();
  int getNumFrames();
  QImage getImage();
  int addText(std::string);
  textInformation *getText(int);
  virtual bool open(const cv::String &);

  bool updateFrame(); // slot for updating the current frame
  void repaintFrame();
  void setToStart();

  virtual void goToPast();
  virtual void goToFuture();
  virtual void createPast(int);
  virtual void clearFuture();

private:
  int fps;
  int numFrames;
  int currFrame;
  std::vector<std::unique_ptr<textInformation>> *textList;
  cv::Mat frame;
  QImage qtImage;
  std::vector<std::unique_ptr<textInformation>> *past;
  std::vector<std::unique_ptr<textInformation>> *future;
};
