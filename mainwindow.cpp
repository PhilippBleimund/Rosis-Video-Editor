#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qobjectdefs.h"
#include <QMessageBox>
#include <QObject>
#include <qmediaplayer.h>
#include <qobject.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), timer(new QTimer(this)), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  ui->graphicsView->setScene(new QGraphicsScene(this));
  ui->graphicsView->scene()->addItem(&pixmap);

  setUpActions();
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
  if (video.isOpened()) {
    video.release();
    event->accept();
  }
}

void MainWindow::setUpActions() {

  // load Video
  QObject::connect(ui->actionLoad_Video, SIGNAL(triggered()), this,
                   SLOT(openFile()));

  // play Video
  QObject::connect(ui->actionPlay_Video, SIGNAL(triggered()), this,
                   SLOT(playPause()));

  // connect timer to update Frames
  QObject::connect(timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
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
    video.open(fileName.toStdString());
    if (!video.isOpened()) {
      qDebug() << "Error: Could not open video.";
      return;
    }

    // Start timer
    timer->start(1000 / video.getFPS());

    ui->actionPlay_Video->setEnabled(true); // Enable the play button
  }
}

void MainWindow::playPause() {
  if (!video.isOpened())
    return;

  if (isPlaying) {
    isPlaying = false;
    mediaPlayer.pause();

    QIcon icon(QIcon::fromTheme(QString::fromUtf8("media-playback-start")));
    ui->actionPlay_Video->setIcon(icon);
  } else {
    isPlaying = true;
    mediaPlayer.play(); // Play the video if it's currently paused

    QIcon icon(QIcon::fromTheme(QString::fromUtf8("media-playback-pause")));
    ui->actionPlay_Video->setIcon(icon);
  }
}

void MainWindow::updateFrame() {
  if (isPlaying && video.isOpened()) {
    isPlaying = this->video.updateFrame();
    if (isPlaying == false)
      return;

    pixmap.setPixmap(QPixmap::fromImage(video.getImage().rgbSwapped()));
    ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
  }
}
