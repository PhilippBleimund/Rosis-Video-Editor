#pragma once

#include "timeline.h"
#include <QFont>
#include <QPoint>
#include <memory>
#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>
#include <pango/pangocairo.h>
#include <qpoint.h>
#include <string>
#include <vector>

#define MAX_SAVES 20

class TextInfoBox;

class textInformation : public timeline {
public:
  textInformation();
  textInformation(std::string, QPoint, QPoint, QFont, cv::Scalar, int, int,
                  TextInfoBox *);
  textInformation(const textInformation *);

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

  void applyDelta();

  virtual void goToPast();
  virtual void goToFuture();
  virtual void createPast();
  virtual void createFuture();

private:
  std::string text;
  QPoint pos;
  QPoint delta;
  QFont fontDesc;
  cv::Scalar color;
  int frameStart;
  int frameEnd;
  TextInfoBox *uiElement;
  std::vector<std::unique_ptr<textInformation>> *past;
  std::vector<std::unique_ptr<textInformation>> *future;
};
