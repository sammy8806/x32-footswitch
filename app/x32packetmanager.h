#ifndef X32PACKETMANAGER_H
#define X32PACKETMANAGER_H

#include <QObject>
#include <oscudpsocket.h>

#include <osc/composer/OscMessageComposer.h>

class X32PacketManager : public QObject
{
    Q_OBJECT
public:
    explicit X32PacketManager(OscUdpSocket *sock, QObject *parent = nullptr);

    void sendPacket(OscMessageComposer *message);

private:
    OscUdpSocket *socket;

};

#endif // X32PACKETMANAGER_H
