#include "x32configuserctrl.h"

// { RotFader, RotPan, RotSend, RotEffect, RotMidi, RotRemote, RotChanSelect }

X32ConfigUserctrl::X32ConfigUserctrl(X32ConsoleAbstract *console, QObject *parent) :
    QObject(parent),
    console(console)
{
    btnMap = new QMap<QString, X32ConfigBtn>();
    btnMap->insert("P", BtnJumpToPage);
    btnMap->insert("O", BtnMute);
    btnMap->insert("I", BtnInsert);
    btnMap->insert("X", BtnEffect);
    btnMap->insert("M", BtnMidi);
    btnMap->insert("R", BtnRemote);
    btnMap->insert("S", BtnRecall);
    btnMap->insert("T", BtnUsbRecorder);
    btnMap->insert("A", BtnAutomixEnable);

    assignData = new QMap<QChar, UserctrlBank*>();
    assignData->insert(QChar('A'), new UserctrlBank());
    assignData->insert(QChar('B'), new UserctrlBank());
    assignData->insert(QChar('C'), new UserctrlBank());

    for(qint8 bank = 0x41; bank <= 0x43; bank++) {
        UserctrlBank* bankData = assignData->value(QChar(bank));
        bankData->bank = QChar(bank);
        bankData->data = new UserctrlBankData();

        for(int i=5; i<=12; i++) {
            UserctrlButton btn;
            bankData->data->insert(i, btn);
        }
    }

    // TODO: EnumTypes => Classes/Structs for Naming etc

/*
    QRegExp configFilter("^/config/userctrl/([a-zA-Z])/btn/([0-9]+)"); // "^([a-zA-Z])(.*)"
    configFilter.indexIn(msg->getAddress());
    QStringList lst = configFilter.capturedTexts();

    // is a button
    if(lst.length() == 3) {
        QString page = lst.at(1);
        QString btn = lst.at(2);

        qDebug() << "ConfigData: " << page << btn;

        this->page = page;
        this->number = btn;

        QString data = msg->getValue(0)->toString();

        if(data.length() > 0) {
            this->buttonData.data = data;
        }
    }*/
}

QString X32ConfigUserctrl::parseButtonData(QString data)
{
    QString output;

    if(data.at(0) == 'O') {
        output = "Mute";
    } else {
        output = "###";
    }

    return output;
}

QString X32ConfigUserctrl::parseChannelName(qint8 channelNumber)
{
    QString output = "";

    // TODO: Return Channel Names if set

    if(channelNumber <= 31) {
        output = "Channel " + channelNumber;
    } else if (channelNumber <= 39) {
        output = "Aux " + QString::number(channelNumber - 31);
    } else if (channelNumber <= 47) {
        bool left = (channelNumber - 39) % 2;
        output = "FX " + QString::number((int)floor((channelNumber - 39)/2)) + QString(left ? "L" : "R");
    } else if (channelNumber <= 63) {
        output = "MixBus " + QString::number(channelNumber - 47);
    } else if (channelNumber <= 69) {
        output = "Matrix " + QString::number(channelNumber - 63);
    } else if (channelNumber <= 70) {
        output = "Main LR";
    } else if (channelNumber <= 71) {
        output = "Main M/C";
    } else if (channelNumber <= 79) {
        output = "DCA " + QString::number(channelNumber - 71);
    } else if (channelNumber <= 85) {
        output = "MuteGroup " + QString::number(channelNumber - 79);
    }

    return output;
}

void X32ConfigUserctrl::updateBtn(UserctrlBank *bank, qint8 btnNr, OscMessage &data)
{
    qDebug() << "Updating Buton: " << btnNr;
    QString btnData = data.getValue(0)->toString();

    if(btnData.length() == 0) return;

    UserctrlButton btn;
    btn.type = this->btnMap->value(btnData.at(0));
    btn.data = btnData;

    qDebug() << "Btn" << btnNr << " => " << btnData;

    bank->data->insert(btnNr, btn);
    emit updatedButton(bank, btnNr);
}
