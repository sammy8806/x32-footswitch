#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateStatus(X32Status status)
{
    qDebug() << "Status: " << status.consoleName;
}

void MainWindow::updateUserctrl(X32ConfigUserctrl config)
{

}
