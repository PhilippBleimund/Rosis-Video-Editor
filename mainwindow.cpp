#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QMessageBox"
#include "QObject"
#include "qobjectdefs.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  QObject::connect(ui->actionLoad_Video, SIGNAL(triggered()), this,
                   SLOT(openDialog()));
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::openDialog() {
  QMessageBox msgBox;
  msgBox.setText("The document has been modified.");
  msgBox.exec();
}
