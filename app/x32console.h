#ifndef X32CONSOLE_H
#define X32CONSOLE_H

#include <QObject>
#include <oscudpsocket.h>

#include <osc/composer/OscMessageComposer.h>

#include "x32Types/x32type.h"
#include "x32Types/channel.h"
#include "x32Types/mutegroup.h"

class X32Console : public QObject
{
    Q_OBJECT
public:
    explicit X32Console(QObject *parent = nullptr);

    void setSocket(OscUdpSocket *socket);
    void sendMessage(OscMessageComposer msg);

    void removeMessage(OscMessage& msg);

protected:
    QMap<qint16, Mutegroup*> *mutegroups;
    QMap<qint16, Channel*> *channels;
    // QMap<qint16, Cue> cues;
    // QMap<qint16, Scene> scenes;

private:
    OscUdpSocket *socket;
    QVector<OscMessage*> *dataPool;

signals:
    X32_INTERNAL void distributeMessage(QString address, OscMessage& dataPtr);

public slots:
    void handleMessage(QNetworkDatagram data);
};

#endif // X32CONSOLE_H
