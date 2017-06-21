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

        OscMessageComposer mute("/config/mute/" + QString::number(number));
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
        mute.pushString("config/mute");
        this->console->sendMessage(mute);
    }


private:
    X32ConsoleAbstract *console;
    QTimer *refreshTimer;

public:
    qint16 number;
    bool state;

public slots:
    X32_INTERNAL void findMessage(QString address, OscMessage& data) {
        if(address.left(12) != "/config/mute") return;
        qDebug() << "Mutegroup relevant";

        if(data.getNumValues() == 6)
            this->state = data.getValue(number - 1)->toInteger();
        else
            this->state = data.getValue(0)->toInteger();

        if(number >= 6)
            this->console->removeMessage(data);
    }
};

#endif // MUTEGROUP_H
