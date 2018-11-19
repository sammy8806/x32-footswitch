#ifndef CONSOLERACK_H
#define CONSOLERACK_H

#include <QObject>
#include <QDebug>
#include <QRegularExpression>

#include "x32Types/x32type.h"
#include "x32console.h"

class ConsoleRack : public QObject
{
    Q_OBJECT
public:
    explicit ConsoleRack(QObject *parent = nullptr);

    void addConsole(X32Console *console);

    X32Console *getConsole(QString name);
    X32Console *getFirstConsole();

    OscUdpSocket* socket;

private:
    // IP-Address, Console
    QMap<QString, X32Console*> *consoles;

    void registerConsole(QHostAddress address, int port, XInfo info);

signals:

public slots:
    void handleMessage(QNetworkDatagram data);

};

#endif // CONSOLERACK_H
