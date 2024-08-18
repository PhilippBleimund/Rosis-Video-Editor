#pragma once

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QWidget>
#include <qevent.h>
#include <qsize.h>

struct move_op {
  QPoint start;
  QPoint delta;
  QPoint transformed;
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
  void mouseReleased();

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
