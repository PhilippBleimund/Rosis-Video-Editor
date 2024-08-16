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
#include <qevent.h>
#include <qobjectdefs.h>

#include "customGraphicsView.h"
#include "textinfobox.h"
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
  void openFile();                     // Slot for opening a video file
  void playPause();                    // Slot for toggling play/pause
  void updateFrame();                  // slot for updating the current frame
  void restartVideo();                 // Slot for restarting the current Video
  void textSelected(TextInfoBox *);    // Slot when text is selected
  void textDeselected(TextInfoBox *);  // Slot when text is deselected
  void textFontUpdated(TextInfoBox *); // Slot when information about
                                       // the text instance is updated
  void textAdded();
  void textTextUpdated();

protected:
  void setUpActions();
  void closeEvent(QCloseEvent *);
  void processVideo();

private:
  Ui::MainWindow *ui;
  QComboBox *stylesComboBox;
  QTimer *timer;
  QGraphicsPixmapItem pixmap;
  videoObj video;
  bool isPlaying = false;
  TextInfoBox *current_selected;
  CustomGraphicsView *graphicsView;
};
#endif // MAINWINDOW_H
