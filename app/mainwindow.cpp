#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->btnData = new QMap<qint8, UserctrlButton*>();
    for(int i=0; i<12; i++) {
        this->btnData->insert(i, new UserctrlButton());
    }
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

    btn.at(number - 1)->setText(config.buttonData.data);
    qDebug() << "Set: " << config.buttonData.data;

    UserctrlButton *data = this->btnData->value(number);
    data->data = config.buttonData.data;
}

void MainWindow::updateChannel(Channel *channel)
{
    qDebug() << "Updated CH" << channel->getNumber();

    QPushButton *btn;

    switch (channel->number) {
    case 1:
        btn = ui->chan01;
        break;
    case 32:
        btn = ui->chan32;
        break;
    default:
        return;
        break;
    }

    qDebug() << "-> Chan " << channel->number;
    if(channel->mix.on == X32BoolState::OFF) {
        btn->setStyleSheet("background-color: red;");
    } else {
        btn->setStyleSheet("background-color: green;");
    }
}

void MainWindow::on_btn5_clicked()
{
    UserctrlButton *btnData = this->btnData->value(5);
    switch (btnData->type) {
    case X32ConfigBtn::BtnMute :
        emit mute(btnData->data.toInt(), 1);
        break;
    default:
        break;
    }
}
