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

void X32ConfigUserctrl::updateBtn(UserctrlBank *bank, qint8 btnNr, OscMessage &data)
{
    qDebug() << "Updating Buton: " << btnNr;
    QString btnData = data.getValue(0)->toString();

    UserctrlButton btn;

    if(btnData.length() != 0) {
        btn.type = this->btnMap->value(btnData.at(0));
        btn.data = btnData;
    } else {
        btn.data = "";
    }

    qDebug() << "Btn" << btnNr << " => " << btnData;

    bank->data->insert(btnNr, btn);
    emit updatedButton(bank, btnNr);
}
