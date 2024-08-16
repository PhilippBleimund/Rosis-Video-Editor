#pragma once

#include <QGraphicsView>
#include <QWidget>
#include <qevent.h>

struct move_op {
  QPoint start;
  int x_delta;
  int y_delta;
};

class CustomGraphicsView : public QGraphicsView {
  Q_OBJECT

public:
  CustomGraphicsView(QWidget *parent = nullptr);
public slots:

signals:
  void mouseMoved(move_op);

protected:
  void mousePressEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);

private:
  QPoint start;
  bool is_pressed;
};
