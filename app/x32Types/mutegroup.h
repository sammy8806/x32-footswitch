#ifndef MUTEGROUP_H
#define MUTEGROUP_H

#include "x32type.h"

#include <QObject>
#include <oscudpsocket.h>

#include <osc/composer/OscMessageComposer.h>

class Mutegroup : public QObject {
    Q_OBJECT

public:
    Mutegroup(X32ConsoleAbstract *console, qint16 number, bool state = false, QObject *parent = nullptr);

    void mute(bool status) {  // mix/on
        qDebug() << "[MuteGroup " + QString::number(number)+1 + "] " + (status ? "" : "Un") + "Mute";

        OscMessageComposer mute("/config/mute/" + number);
        mute.pushInt32((int)status);
        this->console->sendMessage(mute);

        this->refreshTimer->start(50);
    }

    void refresh() {
        this->refreshTimer->start(0);
    }

public slots:
    void timedRefresh() {
        OscMessageComposer mute("/node");
        mute.pushString("config/mute/" + number);
        this->console->sendMessage(mute);
    }


private:
    X32ConsoleAbstract *console;
    QTimer *refreshTimer;

protected:
    qint16 number;
    bool state;

public slots:
    X32_INTERNAL void findMessage(QString address, OscMessage& data) {
        if(address.left(12) != "/config/mute") return;
        qDebug() << "Mutegroup relevant";

        bool ok = false;
        qint8 grp = address.mid(14,1).toInt(&ok);
        if(grp != this->number) return;

        this->state = data.getValue(0)->toInteger();

        this->console->removeMessage(data);
    }
};

#endif // MUTEGROUP_H
