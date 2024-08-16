#include "customGraphicsView.h"

#include <QGraphicsView>
#include <QMouseEvent>
#include <QPoint>
#include <qdebug.h>
#include <qtmetamacros.h>

CustomGraphicsView::CustomGraphicsView(QWidget *parent)
    : QGraphicsView(parent) {
}

void CustomGraphicsView::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    this->start = event->pos();
    is_pressed = true;
  }
}

void CustomGraphicsView::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    is_pressed = false;
  }
}

void CustomGraphicsView::mouseMoveEvent(QMouseEvent *event) {
  if (is_pressed) {
    QPoint pos = event->pos();
    move_op sig;
    sig.start = this->start;
    sig.x_delta = pos.x() - this->start.x();
    sig.y_delta = pos.y() - this->start.y();
    qDebug() << sig.x_delta << "  " << sig.y_delta;

    // emit mouseMoved(sig);
  }
}
