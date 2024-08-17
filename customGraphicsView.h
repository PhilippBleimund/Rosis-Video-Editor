#pragma once

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QWidget>
#include <qevent.h>
#include <qsize.h>

struct move_op {
  QPoint start;
  int x_delta;
  int y_delta;
  int x_transformed;
  int y_transformed;
};

class CustomGraphicsView : public QGraphicsView {
  Q_OBJECT

public:
  CustomGraphicsView(QWidget *parent = nullptr);
  void fitInView(const QGraphicsItem *item,
                 Qt::AspectRatioMode aspectRatioMode = Qt::IgnoreAspectRatio);
public slots:

signals:
  void mouseMoved(move_op);

protected:
  void mousePressEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);

private:
  QPoint calcTransformedPos(QPoint pos, QSize source, QSize view);
  QSize source_size;
  QPoint start;
  QPoint start_transformed;
  bool is_pressed;
};
