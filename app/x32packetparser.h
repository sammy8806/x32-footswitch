#ifndef X32PACKETPARSER_H
#define X32PACKETPARSER_H

#include <QObject>
#include <QDebug>
#include <QVector>

#include "oscudpsocket.h"

#include <osc/reader/OscReader.h>
#include <osc/reader/OscMessage.h>
#include <osc/reader/OscContent.h>
#include <osc/reader/OscBundle.h>

#include <osc/reader/types/OscValue.h>
#include <osc/reader/types/OscFloat.h>

#include <osc/OscPatternMatching.h>

#include <x32Types/x32type.h>
#include <x32Types/x32status.h>
// #include <x32Types/x32configuserctrl.h>

class X32PacketParser : public QObject
{
    Q_OBJECT
public:
    explicit X32PacketParser(OscUdpSocket *socket, QObject *parent = nullptr);

protected:
    void parseStatusData(OscMessage* msg);
    void handleTypeData(OscMessage* msg);

private:
    QVector<X32Type> types;

signals:
    void receivedStatus(X32Status status);
    // void receivedUserctrl(X32ConfigUserctrl config);

public slots:
    void handleOscReader(QNetworkDatagram data);
};

#endif // X32PACKETPARSER_H
