#ifndef CONSOLERACK_H
#define CONSOLERACK_H

#include <QObject>

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
    QMap<QString, XInfo> *getConsoleList();

private:
    // IP-Address, Console
    QMap<QString, X32Console*> *consoles;

    // IP-Address,
    QMap<QString, XInfo> *consoleList;

signals:

public slots:
    void handleMessage(QNetworkDatagram data);

};

#endif // CONSOLERACK_H
