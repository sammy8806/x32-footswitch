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

void X32PacketParser::handleOscReader(QNetworkDatagram data)
{
    QByteArray byteData(data.data());

    OscReader reader(&byteData);
    OscMessage* msg = reader.getMessage();

    QString address = msg->getAddress();	// Get the message address
    qDebug() << address << "#" << msg->getNumValues();

    // handleTypeData(msg);
}
