#ifndef OSCUDPSOCKET_H
#define OSCUDPSOCKET_H

#include <assert.h>

#include <QObject>
#include <QDebug>
#include <QUdpSocket>
#include <QNetworkDatagram>

#include <osc/reader/OscReader.h>
#include <osc/reader/OscMessage.h>
#include <osc/reader/OscContent.h>
#include <osc/reader/OscBundle.h>

#include <osc/reader/types/OscValue.h>
#include <osc/reader/types/OscFloat.h>

#include <osc/composer/OscMessageComposer.h>

class OscUdpSocket : public QObject
{
    Q_OBJECT
public:
    explicit OscUdpSocket(QObject *parent = nullptr);

    void initSocket();
    void processDatagram(QNetworkDatagram data);

    void sendData(QByteArray* data);

private:
    QUdpSocket *udpSocket;
    QString bullshit = "test";

signals:
    void datagramReady(QNetworkDatagram datagram);

public slots:
    void readPendingDatagrams();
    void sendMessage(QByteArray data);
};

#endif // OSCUDPSOCKET_H
