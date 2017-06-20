#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <x32Types/x32type.h>
#include <x32Types/x32configuserctrl.h>

class X32Config : public QObject
{
    Q_OBJECT
public:
    X32Config(X32ConsoleAbstract *console, QObject *parent = nullptr) : QObject(parent), console(console) {
        this->userctrl = new X32ConfigUserctrl(console, this);

        connect(this, SIGNAL(distributeMessage(QString,OscMessage&)), userctrl, SLOT(findMessage(QString,OscMessage&)));
    }

private:
    X32ConsoleAbstract* console;

public:
    ChLink chlink;
    X32ConfigUserctrl *userctrl;

signals:
    X32_INTERNAL void distributeMessage(QString address, OscMessage& dataPtr);
    void updated(X32Config* config);

public slots:
    X32_INTERNAL void findMessage(QString address, OscMessage& data) {
        emit distributeMessage(address, data);
    }
};

#endif // CONFIG_H
