#pragma once

#include <QGraphicsView>
#include <QWidget>

class CustomGraphicsView : public QGraphicsView {
  Q_OBJECT

public:
  CustomGraphicsView(QWidget *parent = nullptr);
public slots:

signals:

protected:
  void mousePressEvent(QMouseEvent *);
};
