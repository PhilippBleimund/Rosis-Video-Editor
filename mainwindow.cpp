#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qobjectdefs.h"
#include "textinfobox.h"
#include "videoObj.h"
#include <QComboBox>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QObject>
#include <qcombobox.h>
#include <qlabel.h>
#include <qmediaplayer.h>
#include <qobject.h>
#include <qpalette.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), timer(new QTimer(this)), ui(new Ui::MainWindow),
      stylesComboBox(new QComboBox(this)) {
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

  // reset Video to start
  QObject::connect(ui->actionStop_Video, SIGNAL(triggered()), this,
                   SLOT(restartVideo()));

  // add text
  QObject::connect(ui->actionAddText, SIGNAL(triggered()), this,
                   SLOT(textAdded()));

  // connect timer to update Frames
  QObject::connect(timer, SIGNAL(timeout()), this, SLOT(updateFrame()));

  // connect style chooser to update video
  QObject::connect(stylesComboBox, SIGNAL(currentIndexChanged()), this,
                   SLOT(updateFontStyle()));
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
    video.updateFrame();
    pixmap.setPixmap(QPixmap::fromImage(video.getImage().rgbSwapped()));
    ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);

    // Start timer
    timer->start(1000 / video.getFPS());

    ui->actionPlay_Video->setEnabled(true); // Enable the play button
  }
}

void MainWindow::textAdded() {
  bool ok;
  QString text = QInputDialog::getMultiLineText(
      this, tr("QInputDialog::getText()"), tr("insert Text"), "", &ok);
  if (ok && !text.isEmpty()) {
    int index = video.addText(text.toStdString());

    TextInfoBox *box =
        new TextInfoBox(video.getText(index), ui->scrollAreaWidgetContents);
    box->setText(text);
    ui->verticalLayout->addWidget(box);
    QObject::connect(box, SIGNAL(selected(TextInfoBox *)), this,
                     SLOT(textSelected(TextInfoBox *)));
    QObject::connect(box, SIGNAL(deselected(TextInfoBox *)), this,
                     SLOT(textDeselected(TextInfoBox *)));
    QObject::connect(box, SIGNAL(updated(TextInfoBox *)), this,
                     SLOT(textUpdated(TextInfoBox *)));

    textUpdated(box);
  }
}

void MainWindow::playPause() {
  if (!video.isOpened())
    return;

  if (isPlaying) {
    isPlaying = false;

    QIcon icon(QIcon::fromTheme(QString::fromUtf8("media-playback-start")));
    ui->actionPlay_Video->setIcon(icon);
  } else {
    isPlaying = true;

    QIcon icon(QIcon::fromTheme(QString::fromUtf8("media-playback-pause")));
    ui->actionPlay_Video->setIcon(icon);
  }
}

void MainWindow::updateFrame() {
  if (isPlaying && video.isOpened()) {
    isPlaying = this->video.updateFrame();
    if (isPlaying == false) {
      QIcon icon(QIcon::fromTheme(QString::fromUtf8("media-playback-start")));
      ui->actionPlay_Video->setIcon(icon);
    }

    pixmap.setPixmap(QPixmap::fromImage(video.getImage().rgbSwapped()));
    ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
  }
}

void MainWindow::restartVideo() {
  if (!video.isOpened())
    return;

  isPlaying = false;
  QIcon icon(QIcon::fromTheme(QString::fromUtf8("media-playback-start")));
  ui->actionPlay_Video->setIcon(icon);
  video.setToStart();

  pixmap.setPixmap(QPixmap::fromImage(video.getImage().rgbSwapped()));
  ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
}

void MainWindow::textSelected(TextInfoBox *) {
}

void MainWindow::textDeselected(TextInfoBox *) {
}

void MainWindow::textUpdated(TextInfoBox *) {
  if (!video.isOpened())
    return;

  this->video.repaintFrame();
  pixmap.setPixmap(QPixmap::fromImage(video.getImage().rgbSwapped()));
  ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
}
