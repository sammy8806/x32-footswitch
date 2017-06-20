#ifndef CHANNEL_H
#define CHANNEL_H

#include "x32type.h"

#include <QObject>
#include <QDebug>

#include <oscudpsocket.h>

#include <osc/composer/OscMessageComposer.h>

class Channel : public QObject {
    Q_OBJECT

public:
    Channel(X32ConsoleAbstract *console, qint8 number, QObject *parent = nullptr): QObject(parent), console(console), number(number) {
        // OscMessageComposer mix("/subscribe");
        // mix.pushString("/ch/01/mix/on");
        // mix.pushInt32(10);
        // this->console->sendMessage(mix);
    }

public slots:
    void mute(bool status) {  // mix/on
        OscMessageComposer mute("/ch/" + QString::number(number) + "/mix/on");
        mute.pushBool(status);
        // this->console->sendMessage(mute);

        // mix.on = X32BoolState::OFF;
    }

private:
    X32ConsoleAbstract* console;

public:
    qint8 getNumber() { return this->number; }

    qint8 number;
    ChannelConfig config;
    ChannelDelay delay;
    ChannelPreamp preamp;
    FxGate gate;
    ChannelDynamic dyn;
    ChannelInsert insert;
    EQ eq;
    ChannelMix mix;

signals:
    void updated(Channel* channel);

public slots:
    X32_INTERNAL void findMessage(QString address, OscMessage& data) {
        // qDebug() << "Channel: " << address;
        if(address.left(3) != "/ch") return; // Is type Channel

        // qDebug() << "Chan " << address.mid(4, 2) << " relevant # " << this->number;

        qint8 tmpChan = address.mid(4, 2).toInt();
        if(tmpChan != this->number) return; // Not for this Channel

        // TODO: Stuff here

        if(address.mid(7, 3) == "mix") {
            qDebug() << 1;
            if(address.mid(11,2) == "on") {
                qDebug() << 1;
                qint8 isOn = data.getValue(0)->toInteger();

                qDebug() << "is " << (isOn ? "on" : "off") << " - " << address;
                mix.on = isOn;
            }
        }

        emit updated(this);

        console->removeMessage(data);
    }
};

#endif // CHANNEL_H
