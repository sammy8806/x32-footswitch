#ifndef OSCUDPSOCKET_H
#define OSCUDPSOCKET_H

#include <assert.h>

#include <QObject>
#include <QDebug>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QQueue>
#include <QTimer>

#include <osc/reader/OscReader.h>
#include <osc/reader/OscMessage.h>
#include <osc/reader/OscContent.h>
#include <osc/reader/OscBundle.h>

#include <osc/reader/types/OscValue.h>
#include <osc/reader/types/OscFloat.h>

#include <osc/composer/OscMessageComposer.h>

#include <default_types.h>

struct OscUdpDatagram {
    QHostAddress targetHost;
    int targetPort;

    QByteArray data;
};

class OscUdpSocket : public QObject
{
    Q_OBJECT

public:
    explicit OscUdpSocket(QObject *parent = nullptr);

    void setAddress(QString address);
    void setName(QString name);
    void initSocket();

    void sendData(QByteArray *data);
    void sendData(QByteArray* data, QHostAddress consoleAddr, int consolePort = 10023);
    void sendData(OscUdpDatagram datagram);

private:
    QString getLogPrefix() {
        return QString("[") +
            this->socketName +
            QString("#") +
            QString::number(this->socketNumber) +
            QString("]");
    }

    // TODO: Route Messages from/to consoles?

    QUdpSocket *udpSocket;
    QString socketName;
    int socketNumber;

    QQueue<OscUdpDatagram>* sendQueue;
    QTimer* sendTimer;
    QString hostAddr;

signals:
    void datagramReady(QNetworkDatagram datagram);

private slots:
    void processSendQueue();

public slots:
    void readPendingDatagrams();
    void sendMessage(QByteArray data);
};

#endif // OSCUDPSOCKET_H
