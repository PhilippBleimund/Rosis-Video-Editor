#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "customGraphicsView.h"
#include "qobjectdefs.h"
#include "textinfobox.h"
#include "videoObj.h"
#include <QComboBox>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QObject>
#include <QString>
#include <qcombobox.h>
#include <qglobal.h>
#include <qlabel.h>
#include <qmediaplayer.h>
#include <qobject.h>
#include <qpalette.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), timer(new QTimer(this)), ui(new Ui::MainWindow),
      stylesComboBox(new QComboBox(this)),
      graphicsView(new CustomGraphicsView(this)) {
  ui->setupUi(this);

  this->setCentralWidget(this->graphicsView);

  this->graphicsView->setScene(new QGraphicsScene(this));
  this->graphicsView->scene()->addItem(&pixmap);

  this->current_selected = nullptr;

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

  // when text changed update frame
  QObject::connect(ui->textEdit, SIGNAL(textChanged()), this,
                   SLOT(textTextUpdated()));

  // when move is detected try to move text
  QObject::connect(graphicsView, SIGNAL(mouseMoved(move_op)), this,
                   SLOT(textMoved(move_op)));

  // when text is released changed get applied
  QObject::connect(graphicsView, SIGNAL(mouseReleased()), this,
                   SLOT(textReleased()));

  QObject::connect(ui->actionUndo, SIGNAL(triggered()), this,
                   SLOT(processUndo()));

  QObject::connect(ui->actionRedo, SIGNAL(triggered()), this,
                   SLOT(processRedo()));
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
    this->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);

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
    int uid = video.addText(text.toStdString());

    TextInfoBox *box =
        new TextInfoBox(video.getText(uid), ui->scrollAreaWidgetContents);
    video.getText(uid)->setUiElement(box);
    ui->verticalLayout->addWidget(box);
    QObject::connect(box, SIGNAL(selected(TextInfoBox *)), this,
                     SLOT(textSelected(TextInfoBox *)));
    QObject::connect(box, SIGNAL(deselected(TextInfoBox *)), this,
                     SLOT(textDeselected(TextInfoBox *)));
    QObject::connect(box, SIGNAL(updated(TextInfoBox *)), this,
                     SLOT(textFontUpdated(TextInfoBox *)));

    textFontUpdated(box);
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
    this->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
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
  this->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
}

void MainWindow::textSelected(TextInfoBox *current) {
  // load text into editor
  this->current_selected = current;
  this->ui->textEdit->setText(QString::fromUtf8(current->getData()->getText()));
}

void MainWindow::textDeselected(TextInfoBox *current) {
  this->ui->textEdit->setText(QString::fromUtf8(""));
  this->current_selected = NULL;
}

void MainWindow::textFontUpdated(TextInfoBox *) {
  if (!video.isOpened())
    return;

  // this->video.createPast(current_selected->getData()->getUid());

  this->video.repaintFrame();
  pixmap.setPixmap(QPixmap::fromImage(video.getImage().rgbSwapped()));
  this->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
}

void MainWindow::textTextUpdated() {
  // this->video.createPast(current_selected->getData()->getUid());
  this->current_selected->setText(ui->textEdit->toPlainText());

  this->video.repaintFrame();
  pixmap.setPixmap(QPixmap::fromImage(video.getImage().rgbSwapped()));
  this->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
}

void MainWindow::textMoved(move_op event) {
  if (video.isOpened() && current_selected != nullptr) {
    current_selected->getData()->setDelta(event.delta);

    this->video.repaintFrame();
    pixmap.setPixmap(QPixmap::fromImage(video.getImage().rgbSwapped()));
    this->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
  }
}

void MainWindow::textReleased() {
  if (video.isOpened() && current_selected != nullptr) {
    this->video.createPast(current_selected->getData()->getUid());
    current_selected->getData()->applyDelta();
  }
}

void MainWindow::processUndo() {
  this->video.goToPast();
  this->video.repaintFrame();
  pixmap.setPixmap(QPixmap::fromImage(video.getImage().rgbSwapped()));
  this->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
}

void MainWindow::processRedo() {
  this->video.goToFuture();
  this->video.repaintFrame();
  pixmap.setPixmap(QPixmap::fromImage(video.getImage().rgbSwapped()));
  this->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
}
