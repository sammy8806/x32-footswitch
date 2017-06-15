#ifndef CHANNEL_H
#define CHANNEL_H

#include "x32type.h"

#include <QObject>
#include <oscudpsocket.h>

#include <osc/composer/OscMessageComposer.h>

class Channel : public QObject {
    Q_OBJECT

public:
    Channel(X32ConsoleAbstract *console, QObject *parent = nullptr) : QObject(parent) {
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

protected:
    qint8 number;
    ChannelConfig config;
    ChannelDelay delay;
    ChannelPreamp preamp;
    FxGate gate;
    ChannelDynamic dyn;
    ChannelInsert insert;
    EQ eq;
    ChannelMix mix;

private:
    X32ConsoleAbstract* console;

public slots:
    X32_INTERNAL void findMessage(QString address, OscMessage& data) {
        // qDebug() << "Channel: " << address;
        if(address.left(3) == "/ch") {
            qDebug() << "Chan relevant";
            console->removeMessage(data);
        }
    }
};

#endif // CHANNEL_H
