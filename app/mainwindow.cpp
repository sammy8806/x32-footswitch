#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    btnData(new QList<UserctrlButton>)
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
    this->setWindowTitle("Conntected: " + status.consoleType);
    ui->consoleData->setText(status.consoleName + " (" + status.consoleVersion + ")");
}

void MainWindow::updateUserctrl(X32ConfigUserctrl config)
{
    QList<QPushButton*> btn = {nullptr, nullptr, nullptr, nullptr, ui->btn5, ui->btn6, ui->btn7, ui->btn8, ui->btn9, ui->btn10};

    qint8 number = config.number.toInt();
    if(number < 5) return;

    btn.at(number-1)->setText(config.buttonData.data);
    qDebug() << "Set: " << config.buttonData.data;
}

void MainWindow::on_btn5_clicked()
{
    UserctrlButton btnData = this->btnData->at(5);
    switch (btnData.type) {
    case X32ConfigRotary::BtnMute :
        emit mute(btnData.data.toInt(), 1);
        break;
    default:
        break;
    }
}
