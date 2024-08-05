#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCloseEvent>
#include <QDebug>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QImage>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QMessageBox>
#include <QPixmap>
#include <QTimer>
#include <qcombobox.h>
#include <qobjectdefs.h>

#include "videoObj.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

public slots:
  void openDialog();
  void openFile();    // Slot for opening a video file
  void playPause();   // Slot for toggling play/pause
  void updateFrame(); // slot for updating the current frame

protected:
  void setUpActions();
  void closeEvent(QCloseEvent *);
  void processVideo();

private:
  Ui::MainWindow *ui;
  QComboBox *stylesComboBox;
  QMediaPlayer mediaPlayer;
  QTimer *timer;
  QGraphicsPixmapItem pixmap;
  videoObj video;
  bool isPlaying = false;
};
#endif // MAINWINDOW_H
