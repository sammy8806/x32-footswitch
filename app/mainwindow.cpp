#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->btnData = new QMap<qint8, UserctrlButton*>();
    for(int i=0; i<12*3; i++) {
        this->btnData->insert(i, new UserctrlButton());
    }

    this->btn = {nullptr, nullptr, nullptr, nullptr, ui->btn5, ui->btn6, ui->btn7, nullptr, ui->btn8, ui->btn9, ui->btn10, nullptr};
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
    QString btnData = bank->data->value(btnNr).data;
    QString btnTitle = X32Console::parseButtonData(btnData, console);
    if(this->btn.at(btnNr - 1) == nullptr) return;

    this->btn.at(btnNr - 1)->setText(btnTitle);
    qDebug() << "Set: " << btnData << btnTitle;

    UserctrlButton *data = this->btnData->value(btnNr);
    data->type = bank->data->value(btnNr).type;
    data->data = btnData;
}

void MainWindow::updateUserctrl(UserctrlBank *bank)
{
    QString targetColor = "";
    if(bank->color == 1) {
        targetColor = "red";
    } else if (bank->color == 2) {
        targetColor = "green";
    } else if (bank->color == 3) {
        targetColor = "yellow";
    } else if (bank->color == 4) {
        targetColor = "blue";
    } else if (bank->color == 5) {
        targetColor = "magenta";
    } else if (bank->color == 6) {
        targetColor = "cyan";
    } else if (bank->color == 7) {
        targetColor = "white";
    } else {
        return;
    }

    for(QPushButton *btn : this->btn) {
        btn->setStyleSheet("background-color: " + targetColor + ";");
    }
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

void MainWindow::processBtnClick(qint8 btn)
{
    qDebug() << "Pressed Btn" << btn;

    UserctrlButton *btnData = this->btnData->value(btn);
    if(btnData == nullptr) return;

    switch (btnData->type) {
    case X32ConfigBtn::BtnMute:
        emit mute(btnData->data.right(2).toInt());
        break;
    case X32ConfigBtn::BtnRecall:
        emit recall(btnData->data.right(3));
        break;
    default:
        break;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "Pressed Key:" << event->key();

    int key = event->key();

    if(key >= Qt::Key_F1 && key <= Qt::Key_F3 ||
        key >= Qt::Key_F5 && key <= Qt::Key_F7) {
        int fKey = key - Qt::Key_F1 + 1;
        qDebug() << "Pressed: F" << fKey;
        processBtnClick(fKey + 4 + ((fKey > 3) ? 1 : 0) );
    }
}

void MainWindow::switchBank(int bank)
{

}

// Layout:
// 5   6   7  |  8
// 9   10  11 |  12

void MainWindow::on_btn5_clicked()
{
    processBtnClick(5);
}

void MainWindow::on_btn6_clicked()
{
    processBtnClick(6);
}

void MainWindow::on_btn7_clicked()
{
    processBtnClick(7);
}

void MainWindow::on_btn8_clicked()
{
    processBtnClick(9);
}

void MainWindow::on_btn9_clicked()
{
    processBtnClick(10);
}

void MainWindow::on_btn10_clicked()
{
    processBtnClick(11);
}
