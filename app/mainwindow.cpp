#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    console(nullptr), rack(nullptr)
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
    if (console == nullptr) {
        ui->consoleData->setText("No console found!");
        return;
    }

    if (this->console != nullptr) {
        QObject::disconnect(this->console, SIGNAL(updateChannel(Channel*)), this, SLOT(updateChannel(Channel*)));
        QObject::disconnect(this->console, SIGNAL(updateUserctrlButton(UserctrlBank*,qint8)), this, SLOT(updateUserctrl(UserctrlBank*,qint8)));
        QObject::disconnect(this->console, SIGNAL(updateUserctrlBank(UserctrlBank*)), this, SLOT(updateUserctrl(UserctrlBank*)));

        QObject::disconnect(this, SIGNAL(mute(qint8)), this->console, SLOT(mute(qint8)));
        QObject::disconnect(this, SIGNAL(recall(QString)), this->console, SLOT(recall(QString)));
    }

    this->console = console;

    ui->consoleData->setText(this->console->getConsoleName());

    QObject::connect(this->console, SIGNAL(updateChannel(Channel*)), this, SLOT(updateChannel(Channel*)), Qt::UniqueConnection);
    QObject::connect(this->console, SIGNAL(updateUserctrlButton(UserctrlBank*,qint8)), this, SLOT(updateUserctrl(UserctrlBank*,qint8)), Qt::UniqueConnection);
    QObject::connect(this->console, SIGNAL(updateUserctrlBank(UserctrlBank*)), this, SLOT(updateUserctrl(UserctrlBank*)), Qt::UniqueConnection);

    QObject::connect(this, SIGNAL(mute(qint8)), this->console, SLOT(mute(qint8)), Qt::UniqueConnection);
    QObject::connect(this, SIGNAL(recall(QString)), this->console, SLOT(recall(QString)), Qt::UniqueConnection);
    QObject::connect(this, SIGNAL(insert(qint8)), this->console, SLOT(insert(qint8)));

    for(Channel* chan : *this->console->channels) {
        this->updateChannel(chan);
    }

    for(UserctrlBank* bank : *this->console->config->userctrl->getBanks()) {
        for(quint8 i=0; i<bank->data->size(); i++) {
            updateUserctrl(bank, i+1);
        }
    }
}

void MainWindow::setConsoleRack(ConsoleRack *rack)
{
    this->rack = rack;
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
    if(bank->bank != 'A') {
        return;
    }

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
    if(bank->bank != 'A') {
        return;
    }

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
        if(btn != nullptr) {
            btn->setStyleSheet("background-color: " + targetColor + ";");
        }
    }
}

void MainWindow::updateChannel(Channel *channel)
{
    qDebug() << "Updated CH" << channel->getNumber();

    QPushButton *btn;

    switch (channel->number) {
    // TODO: Channel association from map here
    default:
        qDebug() << "Requested Channel " << channel->getNumber() << " not registered!";
        return;
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
    case X32ConfigBtn::BtnInsert:
        emit insert(btnData->data.right(2).toInt());
        break;
    default:
        break;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "Pressed Key:" << event->key();

    int key = event->key();

    if( (key >= Qt::Key_F1 && key <= Qt::Key_F3) ||
        (key >= Qt::Key_F5 && key <= Qt::Key_F7)
    ) {
        int fKey = key - Qt::Key_F1 + 1;
        qDebug() << "Pressed: F" << fKey;

        // Skip from Btn7 to Btn9
        processBtnClick(fKey + 4 + ((fKey > 3) ? 1 : 0) );
    }

    // TODO: register dialog-window here
    // TODO: register bank-changes here
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

void MainWindow::on_btnSearchConsole_clicked()
{
    if(rack == nullptr) {
        ui->consoleData->setText("No rack found!");
        return;
    }

    X32Console *console = rack->getFirstConsole();

    if(console == nullptr) {
        ui->consoleData->setText("No console found in rack");
        return;
    }

    this->setConsole(console);
}
