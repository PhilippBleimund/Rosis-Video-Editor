#include "customGraphicsView.h"

#include <QGraphicsView>
#include <QMouseEvent>
#include <QPoint>
#include <QRectF>
#include <qdebug.h>
#include <qglobal.h>
#include <qnamespace.h>
#include <qpoint.h>
#include <qsize.h>
#include <qtmetamacros.h>

CustomGraphicsView::CustomGraphicsView(QWidget *parent)
    : QGraphicsView(parent) {
}

void CustomGraphicsView::fitInView(const QGraphicsItem *item,
                                   Qt::AspectRatioMode aspectRatioMode) {
  // custom logic
  QRectF size = item->boundingRect();
  this->source_size.setWidth(size.width());
  this->source_size.setHeight(size.height());

  // continue with default behaviour
  QGraphicsView::fitInView(item, aspectRatioMode);
}

void CustomGraphicsView::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    this->start = event->pos();
    this->start_transformed =
        calcTransformedPos(this->start, this->source_size, this->size());
    is_pressed = true;
  }
}

void CustomGraphicsView::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    is_pressed = false;
  }
}

QPoint CustomGraphicsView::calcTransformedPos(QPoint pos, QSize source,
                                              QSize view) {
  QPoint new_pos;
  QSize scaled_source = QSize(source);
  scaled_source.scale(view, Qt::KeepAspectRatio);

  int x_white = (view.width() - scaled_source.width()) / 2;
  int y_white = (view.height() - scaled_source.height()) / 2;

  // check if in frame
  if (pos.x() < x_white || pos.x() > (x_white + scaled_source.width()) ||
      pos.y() < y_white || pos.y() > (y_white + scaled_source.height())) {
    new_pos.setX(-1);
    new_pos.setY(-1);
    return new_pos;
  }

  // compensate for white space
  int new_x = pos.x() - ((view.width() - scaled_source.width()) / 2);
  int new_y = pos.y() - ((view.height() - scaled_source.height()) / 2);
  // scale to source size
  new_x = (((float)new_x) / view.width()) * source.width();
  new_y = (((float)new_y) / view.height()) * source.height();

  new_pos.setX(new_x);
  new_pos.setY(new_y);

  return new_pos;
}

void CustomGraphicsView::mouseMoveEvent(QMouseEvent *event) {
  if (is_pressed) {
    QPoint pos = event->pos();
    QPoint new_pos = calcTransformedPos(pos, this->source_size, this->size());
    if (new_pos.x() < 0 && new_pos.y() < 0) {
      return;
    }
    move_op sig;
    sig.start = this->start;
    sig.x_transformed = new_pos.x();
    sig.y_transformed = new_pos.y();
    sig.x_delta = new_pos.x() - this->start_transformed.x();
    sig.y_delta = new_pos.y() - this->start_transformed.y();

    qDebug() << sig.x_delta << "  " << sig.y_delta;
    qDebug() << sig.x_transformed << "  " << sig.y_transformed;

    emit mouseMoved(sig);
  }
}
