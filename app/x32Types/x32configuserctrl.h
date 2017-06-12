#ifndef X32CONFIGUSERCTRL_H
#define X32CONFIGUSERCTRL_H

#include <QDebug>
#include <QRegExp>

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

class X32ConfigUserctrl
{
public:
    X32ConfigUserctrl(OscMessage* msg);

    static QRegExp typeRegExp() {
        return QRegExp("^/config/userctrl.*");
    }

    void parseUserCtrl(QString data);

    UserctrlButton buttonData;
    QString page;
    QString number;

};

#endif // X32CONFIGUSERCTRL_H
