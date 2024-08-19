#pragma once

#include "timeObj.h"
#include <QFont>
#include <QPoint>
#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>
#include <pango/pangocairo.h>
#include <qpoint.h>
#include <string>

#define MAX_SAVES 20

class TextInfoBox;

class textInformation : public timeObj {
public:
  textInformation() {};
  textInformation(std::string, QPoint, QPoint, QFont, cv::Scalar, int, int,
                  TextInfoBox * = nullptr);
  textInformation(const textInformation *, bool = false);
  void copyFrom(const textInformation *);

  std::string getText();
  QPoint getPosition();
  cv::Point2d getPosition_as_cvPoint();
  QPoint getDelta();
  QFont getFont();
  PangoFontDescription *getFont_as_Pango();
  cv::Scalar getColor();
  int getFrameStart();
  int getFrameEnd();
  TextInfoBox *getUiElement();

  void setText(std::string);
  void setFont(QFont);
  void setDelta(QPoint);
  void setUiElement(TextInfoBox *);

  void applyDelta(QPoint = QPoint(0, 0));

private:
  std::string text;
  QPoint pos;
  QPoint delta;
  QFont fontDesc;
  cv::Scalar color;
  int frameStart;
  int frameEnd;
  TextInfoBox *uiElement;
};
