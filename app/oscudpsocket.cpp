#include "oscudpsocket.h"

OscUdpSocket::OscUdpSocket(QObject *parent) : QObject(parent), sendQueue(new QQueue<QByteArray*>()), sendTimer(new QTimer())
{

}

void OscUdpSocket::initSocket()
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress("172.16.0.240"), 10050);

    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
    connect(sendTimer, SIGNAL(timeout()), this, SLOT(processSendQueue()));
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
