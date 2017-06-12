#include "x32packetmanager.h"

X32PacketManager::X32PacketManager(OscUdpSocket *sock, QObject *parent) : QObject(parent),
    socket(sock)
{

}

void X32PacketManager::sendPacket(OscMessageComposer *message)
{
    socket->sendData(message->getBytes());
}
