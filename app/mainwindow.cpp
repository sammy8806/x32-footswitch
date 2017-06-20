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

void MainWindow::setConsole(X32Console *console)
{
    this->console = console;
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

void MainWindow::updateUserctrl(UserctrlBank *bank, qint8 btnNr)
{
    QList<QPushButton*> btn = {nullptr, nullptr, nullptr, nullptr, ui->btn5, ui->btn6, ui->btn7, ui->btn8, ui->btn9, ui->btn10};

    QString btnData = bank->data->value(btnNr).data;
    btn.at(btnNr - 1)->setText(btnData);
    qDebug() << "Set: " << btnData;

    UserctrlButton *data = this->btnData->value(btnNr);
    data->type = bank->data->value(btnNr).type;
    data->data = btnData;
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
    if(!channel->mix.on) {
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
        emit mute(btnData->data.right(2).toInt());
        break;
    default:
        break;
    }
}
