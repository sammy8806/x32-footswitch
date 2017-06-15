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

private:
    X32ConsoleAbstract *console;

protected:
    qint16 number;
    bool state;

public slots:
    X32_INTERNAL void findMessage(QString address, OscMessage& data) {
        if(address.left(12) == "/config/mute") {
            qDebug() << "Mutegroup relevant";
            this->console->removeMessage(data);
        }
    }
};

#endif // MUTEGROUP_H
