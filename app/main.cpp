#include "mainwindow.h"
#include <QApplication>

#include "oscudpsocket.h"
#include "x32packetparser.h"
#include "backgroundheartbeat.h"

#include <osc/composer/OscMessageComposer.h>
#include <x32console.h>
#include "consolerack.h"

#include <QNetworkInterface>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    OscUdpSocket *sock = new OscUdpSocket();
    ConsoleRack *rack = new ConsoleRack();
    rack->socket = sock;
    sock->initSocket();

    QObject::connect(sock, SIGNAL(datagramReady(QNetworkDatagram)), rack, SLOT(handleMessage(QNetworkDatagram)));

    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);

    QList<QHostAddress> broadcastAddresses = QList<QHostAddress>();

    for(const QNetworkInterface &interface: QNetworkInterface::allInterfaces()) {
        if(
                interface.type() != QNetworkInterface::Wifi
                && interface.type() != QNetworkInterface::Ethernet
        ) {
            continue;
        }

        for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost) {
                qDebug() << "Interface " << interface.name() << ": "
                         // << "[" << interface.type() << "] "
                         << ": " << address.toString();

                for(QNetworkAddressEntry entry : interface.addressEntries()) {
                    QHostAddress localBroadcast = entry.broadcast();
                    if(localBroadcast.isNull()) {
                        continue;
                    }

                    qDebug() << "Broadcast: " << localBroadcast.toString();
                    broadcastAddresses.append(localBroadcast);
                }
            }
        }
    }

    for(QHostAddress broadcast : broadcastAddresses) {
        for (int i=0; i<2; i++) // Throw 2 discovery packets out there
        {
            OscMessageComposer msg("/xinfo");
            OscUdpDatagram discoverPacket;
            discoverPacket.targetHost = broadcast;
            discoverPacket.targetPort = 10023;
            discoverPacket.data = QByteArray(msg.getBytes()->data(), msg.getBytes()->length());

            qDebug() << "Send discover to: " << broadcast.toString();
            sock->sendData(discoverPacket);
        }
    }

    for (int i=0; i<2; i++) // Throw 2 discovery packets out there
    {
        OscMessageComposer msg("/xinfo");
        OscUdpDatagram discoverPacket;
        discoverPacket.targetHost = QHostAddress(QHostAddress::Broadcast);
        discoverPacket.targetPort = 10023;
        discoverPacket.data = QByteArray(msg.getBytes()->data(), msg.getBytes()->length());

        qDebug() << "Send discover to: " << discoverPacket.targetHost.toString();
        sock->sendData(discoverPacket);
    }

    w.setConsoleRack(rack);


    // sock->initSocket();
    // console->refreshValues();

    {
        OscMessageComposer msg("/xinfo");
        sock->sendData(msg.getBytes());
    }

    /*{
        OscMessageComposer msg("/node");
        msg.pushString("ch/02/mix/on");
        sock->sendData(msg.getBytes());
    }*/
/*    {
        OscMessageComposer msg("/unsubscribe");
        sock->sendData(msg.getBytes());
    }
*/

    /* {
        OscMessageComposer msg("/xremote");
        sock->sendData(msg.getBytes());
    }

    for(char bank = 'A'; bank <= 'C'; bank++) {
        OscMessageComposer msg("/config/userctrl/" + QString(bank) + "/color");
        sock->sendData(msg.getBytes());

        for(int i=5; i<12; i++) {
            OscMessageComposer msg("/config/userctrl/" + QString(bank) + "/btn/" + QString::number(i));
            sock->sendData(msg.getBytes());
        }
    } */

/*
    {
        OscMessageComposer msg("/config/userctrl/A/btn/5");
        msg.pushString("O02");
        sock->sendData(msg.getBytes());
    }*/

    /*
    OscMessageComposer msg2( "/ch/02/mix/fader");
    msg2.pushFloat(0.4692);
    sock->sendData(msg2.getBytes());
    */

    w.showMaximized();
    // w.showMinimized();
    // w.showNormal();

    return a.exec();
}
