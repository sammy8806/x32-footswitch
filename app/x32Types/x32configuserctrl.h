#ifndef X32CONFIGUSERCTRL_H
#define X32CONFIGUSERCTRL_H

#include <QObject>
#include <QDebug>
#include <QRegExp>
#include <assert.h>

#include <x32Types/x32type.h>

#include <osc/reader/OscMessage.h>
#include <osc/reader/types/OscValue.h>
#include <osc/reader/types/OscString.h>
#include <osc/reader/types/OscFloat.h>
#include <osc/reader/types/OscInteger.h>

enum X32ConfigBtn : char {
    BtnJumpToPage = 'P',
    BtnMute = 'O',
    BtnInsert = 'I',
    BtnEffect = 'X',
    BtnMidi = 'M', // MidiPush & MidiToggle
    BtnRemote = 'R',
    BtnRecall = 'S', // 3 Types of Recall (Scene=0, Snippet=2, Cue=4)
    BtnUsbRecorder = 'T',
    BtnAutomixEnable = 'A'
};

enum X32ConfigRotary : char {
    RotFader = 'F',
    RotPan = 'P',
    RotSend = 'S',
    RotEffect = 'X',
    RotMidi = 'M',
    RotRemote = 'R',
    RotChanSelect = 'D'
};

struct UserctrlButton {
    X32ConfigBtn type;
    QString data;
};

typedef QMap<qint8, QString> UserctrlBank;

class X32ConfigUserctrl : public QObject
{
    Q_OBJECT

public:
    explicit X32ConfigUserctrl(X32ConsoleAbstract *console, QObject *parent = nullptr);

    static QRegExp typeRegExp() {
        return QRegExp("^/config/userctrl.*");
    }

    void parseUserCtrl(QString data);

    UserctrlButton buttonData;
    QString page;
    QString number;

    QMap<QString, X32ConfigBtn> *btnMap;
    QMap<QString, X32ConfigRotary> *rotaryMap;

private:
    QMap<QChar, UserctrlBank*> *assignData;
    X32ConsoleAbstract *console;

    void updateBtn(UserctrlBank* bank, qint8 btnNr, OscMessage& data);

signals:
    void updated(X32ConfigUserctrl* userctrl);

public slots:
    X32_INTERNAL void findMessage(QString address, OscMessage& data) {
        if(address.left(16) != "/config/userctrl") return;

        QChar targetBank = address.at(17);
        UserctrlBank *bank = assignData->value(targetBank);

        if(address.mid(19, 3) == "btn") {
            qint8 btnNr = address.mid(23, 2).toInt();
            assert(btnNr != 0);
            updateBtn(bank, btnNr, data);
        }
    }

};

#endif // X32CONFIGUSERCTRL_H
