#include "oscudpsocket.h"

OscUdpSocket::OscUdpSocket(QObject *parent) : QObject(parent)
{

}

void OscUdpSocket::initSocket()
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress("172.16.0.240"), 10050);

    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

// useless !
void OscUdpSocket::processDatagram(QNetworkDatagram data)
{
    qDebug() << "Data: " << data.data();

    QByteArray byteData(data.data());

    OscReader reader(&byteData);
    OscMessage* msg = reader.getMessage();

    QString address = msg->getAddress();	// Get the message address
    qDebug() << address << "#" << msg->getNumValues();

    delete msg;
}

void OscUdpSocket::sendData(QByteArray *data)
{
    if(this->udpSocket == nullptr) {
        qDebug() << "What happened here?!";
        this->initSocket();
    }

    assert(this->udpSocket != nullptr);

    qint64 bytesWritten = this->udpSocket->writeDatagram(*data, QHostAddress("172.16.1.100"), 10023);

    // qint64 bytesWritten = udpSocket->write(*data);
    if(bytesWritten < 0) {
        qDebug() << "Data write failed!";
    } else {
        qDebug() << "Written: " << bytesWritten << " bytes";
    }
}

void OscUdpSocket::readPendingDatagrams()
{
    while(udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        emit datagramReady(datagram);
    }
}

void OscUdpSocket::sendMessage(QByteArray data)
{
    this->sendData(&data);
}
