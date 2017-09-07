#include "oscudpsocket.h"

OscUdpSocket::OscUdpSocket(QObject *parent) : QObject(parent), sendQueue(new QQueue<QByteArray*>()), sendTimer(new QTimer()), hostAddr(QString())
{
    this->socketNumber = socketCount++;
}

void OscUdpSocket::setAddress(QString address)
{
    this->hostAddr = address;
}

void OscUdpSocket::setName(QString name)
{
    this->socketName = name;
}

void OscUdpSocket::initSocket()
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress(QHostAddress::Any), 10050, QUdpSocket::ReuseAddressHint);

    if(udpSocket->state() == QUdpSocket::BoundState) {
        DebugLog << "Socket bound!";
    } else {
        DebugLog << "Connection failed!";
    }

    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
    connect(sendTimer, SIGNAL(timeout()), this, SLOT(processSendQueue()));

    if(this->hostAddr == "") {
        this->hostAddr = "172.16.1.100";

        QString envHost = QString(qgetenv("X32HOST"));
        if(envHost != "") {
            DebugLog << "Using envvar X32HOST for target address";
            this->hostAddr = envHost;
        }

        DebugLog << "Setting connection Address to: " << this->hostAddr;
    } else {
        DebugLog << "Using preset address: " << this->hostAddr;
    }

    if(this->socketName == "") {
        DebugLog << "Name empty ... using " << this->hostAddr;
        this->socketName = this->hostAddr;
    }
}

void OscUdpSocket::sendData(QByteArray *data)
{
    QByteArray *dat = new QByteArray(data->data(), data->length());
    this->sendQueue->enqueue(dat);
    this->sendTimer->start(7);
}

void OscUdpSocket::processSendQueue()
{
    QByteArray* data = sendQueue->dequeue();

    if(sendQueue->length() == 0)
        sendTimer->stop();

    if(this->udpSocket == nullptr) {
        DebugLog << "What happened here?!";
        this->initSocket();
    }

    assert(this->udpSocket != nullptr);

    qint64 bytesWritten = this->udpSocket->writeDatagram(*data, QHostAddress(this->hostAddr), 10023);

    // qint64 bytesWritten = udpSocket->write(*data);
    if(bytesWritten < 0) {
        DebugLog << "Data write failed!";
    } else {
        DebugLog << "Written:" << bytesWritten << "bytes";
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
