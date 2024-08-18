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
    if (event->pos() != this->start) {
      emit mouseReleased();
    }
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
  float new_x = static_cast<float>(pos.x() - x_white);
  float new_y = static_cast<float>(pos.y() - y_white);
  new_x = (new_x / static_cast<float>(scaled_source.width())) *
          static_cast<float>(source.width());
  new_y = (new_y / static_cast<float>(scaled_source.height())) *
          static_cast<float>(source.height());

  new_pos.setX(static_cast<int>(std::round(new_x)));
  new_pos.setY(static_cast<int>(std::round(new_y)));

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
    sig.transformed = new_pos;
    sig.delta = new_pos - this->start_transformed;

    emit mouseMoved(sig);
  }
}
