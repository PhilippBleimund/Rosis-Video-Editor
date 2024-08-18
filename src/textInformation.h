#pragma once

#include <QFont>
#include <QPoint>
#include <QVector2D>
#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>
#include <pango/pangocairo.h>
#include <string>

#include "textinfobox.h"

class textInformation {
public:
  textInformation();
  textInformation(std::string, QPoint, QVector2D, QFont, cv::Scalar, int, int,
                  TextInfoBox *);

  std::string getText();
  QPoint getPosition();
  cv::Point2d getPosition_as_cvPoint();
  QVector2D getDelta();
  QFont getFont();
  PangoFontDescription *getFont_as_Pango();
  cv::Scalar getColor();
  int getFrameStart();
  int getFrameEnd();
  TextInfoBox *getUiElement();

  void applyDelta();

private:
  std::string text;
  QPoint pos;
  QVector2D delta;
  QFont fontDesc;
  cv::Scalar color;
  int frameStart;
  int frameEnd;
  TextInfoBox *uiElement;
};
