#ifndef CHANNEL_H
#define CHANNEL_H

#include "x32type.h"

#include <QObject>
#include <QDebug>
#include <QTimer>

#include <oscudpsocket.h>

#include <osc/composer/OscMessageComposer.h>

class Channel : public QObject {
    Q_OBJECT

public:
    Channel(X32ConsoleAbstract *console, quint8 number, QObject *parent = nullptr): QObject(parent),
        console(console), refreshTimer(new QTimer()), number(number) {
        // OscMessageComposer mix("/subscribe");
        // mix.pushString("/ch/01/mix/on");
        // mix.pushInt32(10);
        // this->console->sendMessage(mix);

        insert.on = false;

        props = new QList<QString>();
        props->append("config/name");
        props->append("config/icon");
        props->append("config/color");
        props->append("config/source");
        props->append("mix/on");
        props->append("mix/fader");
        props->append("mix/st");
        props->append("mix/pan");
        props->append("mix/mono");
        props->append("insert/on");
        // props->append("insert/pos");
        // props->append("insert/sel");
        // props->append("grp/dca");
        // props->append("grp/mute");

        connect(refreshTimer, SIGNAL(timeout()), this, SLOT(timedRefresh()));
        this->refreshTimer->setSingleShot(true);
    }

    void mute(bool status) {  // mix/on
        qDebug() << "[CH" + QString::number(number)+1 + "] " + (status ? "" : "Un") + "Mute";

        QString chan = QString("%1").arg(number, 2, 10, QChar('0'));
        OscMessageComposer mute("/ch/" + chan + "/mix/on");
        mute.pushInt32((int)status);
        this->console->sendMessage(mute);

        this->refreshTimer->start(50);
    }

    void setInsert(bool status) { // insert/on
        qDebug() << "[CH" + QString::number(number)+1 + "] " + (status ? "" : "Not ") + "Inserted";

        QString chan = QString("%1").arg(number, 2, 10, QChar('0'));
        OscMessageComposer insert("/ch/" + chan + "/insert/on");
        insert.pushInt32((int)status);
        this->console->sendMessage(insert);

        this->refreshTimer->start(50);
    }

    void refresh() {
        this->refreshTimer->start(0);
    }

public slots:
    void timedRefresh() {
        QString chan = QString("%1").arg(number, 2, 10, QChar('0'));

        foreach(QString prop, *props) {
            OscMessageComposer mute("/node");
            mute.pushString("ch/" + chan + "/" + prop);
            this->console->sendMessage(mute);
        }
    }


private:
    X32ConsoleAbstract* console;
    QList<QString> *props;
    QTimer *refreshTimer;

    bool stateDirty = false;

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

        if(address.mid(7, 6) == "config") {
            if(address.mid(14, 4) == "name") {
                this->stateDirty = true;

                if(data.getNumValues() >= 1) {
                    QString name = data.getValue(0)->toString();
                    name.remove('"');
                    name.remove('"');
                    qDebug() << "[Channel "+QString::number(this->number)+"] Name: " << name.trimmed();
                    config.name = name.trimmed();
                }
            }
        }

        if(address.mid(7, 3) == "mix") {
            if(address.mid(11,2) == "on") {
                this->stateDirty = true;

                bool isOn;
                QString isOnTmp = data.getValue(0)->toString();

                if(isOnTmp == "ON") {
                    isOn = true;
                } else if(isOnTmp == "OFF") {
                    isOn = false;
                } else {
                    isOn = data.getValue(0)->toBoolean();
                }

                qDebug() << "is " << (isOn ? "on" : "off") << " - " << address;
                mix.on = isOn;
            }
        }

        if(address.mid(7, 6) == "insert") {
            if(address.mid(14, 2) == "on") {
                this->stateDirty = true;

                bool isOn = false;
                QString isOnTmp = data.getValue(0)->toString();

                if(isOnTmp == "ON") {
                    isOn = true;
                } else if(isOnTmp == "OFF") {
                    isOn = false;
                } else {
                    isOn = data.getValue(0)->toBoolean();
                }

                qDebug() << "is " << (isOn ? "inserted" : "not inserted") << " - " << address;
                insert.on = isOn;
            }
        }

        if(this->stateDirty) {
            emit updated(this);
            this->stateDirty = false;
        }

        console->removeMessage(data);
    }
};

#endif // CHANNEL_H
