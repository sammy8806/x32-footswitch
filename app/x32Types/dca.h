#ifndef DCA_H
#define DCA_H

#include "x32type.h"

#include <QObject>
#include <oscudpsocket.h>

#include <osc/composer/OscMessageComposer.h>

class Dca : public QObject
{
    Q_OBJECT

public:
    Dca(X32ConsoleAbstract *console, quint8 number, bool active = false, QObject *parent = nullptr);

    void mute(bool status) {  // <number>/on
        qDebug() << "[DCA " + QString::number(number)+1 + "] " + (status ? "" : "Un") + "Mute";

        OscMessageComposer mute("/dca/" + QString::number(number) + "/on");
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
        mute.pushString("dca/" + QString::number(number) + "/on");
        this->console->sendMessage(mute);
    }


private:
    X32ConsoleAbstract *console;
    QTimer *refreshTimer;

public:
    qint16 number;
    QString name;
    bool active;

public slots:
    X32_INTERNAL void findMessage(QString address, OscMessage& data) {
        if(address.left(6) != "/dca/" + QString::number(number)) return;
        qDebug() << "DCA relevant (" + QString::number(number) + ")";

        if(address.mid(8,11) == "config/name") {
            this->name = data.getValue(0)->toString();
            qDebug() << "Name set to:" << this->name;
        }

        if(address.mid(8,6) == "mix/on") {
            this->active = data.getValue(0)->toInteger();
            qDebug() << "State set to:" << this->active;
        }

        this->console->removeMessage(data);
    }
};


#endif // DCA_H
