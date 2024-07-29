#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QMessageBox"
#include "QObject"
#include "qmediaplayer.h"
#include "qobjectdefs.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), videoPlayer(new QMediaPlayer()),
      ui(new Ui::MainWindow) {
  ui->setupUi(this);

  setUpActions();

  this->videoPlayer->setVideoOutput(ui->video_widget);
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::setUpActions() {

  // load Video
  QObject::connect(ui->actionLoad_Video, SIGNAL(triggered()), this,
                   SLOT(openFile()));

  // play Video
  QObject::connect(ui->actionPlay_Video, SIGNAL(triggered()), this,
                   SLOT(playPause()));
}

void MainWindow::openDialog() {
  QMessageBox msgBox;
  msgBox.setText("The document has been modified.");
  msgBox.exec();
}

void MainWindow::openFile() {
  // Use QFileDialog to get the video file path from the user
  QString fileName =
      QFileDialog::getOpenFileName(this, tr("Open Movie"), QDir::homePath());
  if (!fileName.isEmpty()) {
    videoPlayer->setSource(
        QUrl::fromLocalFile(fileName)); // Set the source for the media player
    ui->actionPlay_Video->setEnabled(true); // Enable the play button
  }
}

void MainWindow::playPause() {
  if (videoPlayer->playbackState() == QMediaPlayer::PlayingState) {
    videoPlayer->pause(); // Pause the video if it's currently playing

    QIcon icon(QIcon::fromTheme(QString::fromUtf8("media-playback-start")));
    ui->actionPlay_Video->setIcon(icon);
  } else {
    videoPlayer->play(); // Play the video if it's currently paused

    QIcon icon(QIcon::fromTheme(QString::fromUtf8("media-playback-pause")));
    ui->actionPlay_Video->setIcon(icon);
  }
}
