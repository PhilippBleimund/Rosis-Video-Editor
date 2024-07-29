#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>

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
  void openFile();  // Slot for opening a video file
  void playPause(); // Slot for toggling play/pause

private:
  Ui::MainWindow *ui;

  QMediaPlayer *videoPlayer;

  void setUpActions();
};
#endif // MAINWINDOW_H
