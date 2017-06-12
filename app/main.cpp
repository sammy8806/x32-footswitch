#include "mainwindow.h"
#include <QApplication>

#include "oscudpsocket.h"
#include "x32packetparser.h"

#include <osc/composer/OscMessageComposer.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    OscUdpSocket *sock = new OscUdpSocket();
    X32PacketParser *parser = new X32PacketParser(sock);

    QObject::connect(parser, SIGNAL(receivedStatus(X32Status)), &w, SLOT(updateStatus(X32Status)));

    sock->initSocket();

    {
        OscMessageComposer msg("/info");
        sock->sendData(msg.getBytes());
    }
/*
    {
        OscMessageComposer msg("/node");
        msg.pushString("ch/02/config");
        sock->sendData(msg.getBytes());
    }

    {
        OscMessageComposer msg("/ch/02/mix/fader");
        sock->sendData(msg.getBytes());
    }
*/
    {
        OscMessageComposer msg("/config/userctrl/A/btn/5");
        sock->sendData(msg.getBytes());
    }
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

    return a.exec();
}
