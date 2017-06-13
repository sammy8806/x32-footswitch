#include "x32packetparser.h"

X32PacketParser::X32PacketParser(OscUdpSocket *socket, QObject *parent) : QObject(parent),
    types(QVector<X32Type>())
{
    connect(socket, SIGNAL(datagramReady(QNetworkDatagram)), this, SLOT(handleOscReader(QNetworkDatagram)));
}

void X32PacketParser::parseStatusData(OscMessage *msg)
{
    for(int i=0; i<4; i++)
        qDebug() << "info#" << i << " " << msg->getValue(i)->toString();
}

void X32PacketParser::handleTypeData(OscMessage *msg)
{
    QString addr = msg->getAddress();

    // /status
    // /xstatus
    // /info
    QRegExp x32status = X32Status::typeRegExp();
    if(x32status.exactMatch(addr)) {
        qDebug() << "PacketType: X32Status";

        X32Status status(msg);
        qDebug() << "Console: " << status.consoleName << "(" << status.consoleVersion << ")";
        emit receivedStatus(status);
    }

    // /config/userctrl
    QRegExp x32userctrl = X32ConfigUserctrl::typeRegExp();
    if(x32userctrl.exactMatch(addr)) {
        qDebug() << "PacketType: X32ConfigUserctrl";

        X32ConfigUserctrl config(msg);
        emit receivedUserctrl(config);
    }
}

void X32PacketParser::handleOscReader(QNetworkDatagram data)
{
    QByteArray byteData(data.data());

    OscReader reader(&byteData);
    OscMessage* msg = reader.getMessage();

    QString address = msg->getAddress();	// Get the message address
    qDebug() << address << "#" << msg->getNumValues();

    emit
    // handleTypeData(msg);
}
