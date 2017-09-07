#include "consolerack.h"

ConsoleRack::ConsoleRack(QObject *parent) : QObject(parent), consoles(new QMap<QString, X32Console*>())
{

}

X32Console *ConsoleRack::getConsole(QString name)
{
    return this->consoles->find(name).value();
}

X32Console *ConsoleRack::getFirstConsole()
{
    return this->consoles->first();
}

QMap<QString, XInfo> *ConsoleRack::getConsoleList()
{
    return this->consoleList;
}

void ConsoleRack::handleMessage(QNetworkDatagram data)
{
    QByteArray byteData(data.data());

    OscReader reader(&byteData);
    OscMessage* msg = reader.getMessage();
    QString address = msg->getAddress();

    if(address.left(6) != "/xinfo") {
        return;
    }

    // /xinfo => <ip> <name> <model> <version>
    QString ip = msg->getValue(0)->toString();
    QString name = msg->getValue(1)->toString();
    QString model = msg->getValue(2)->toString();
    QString version = msg->getValue(3)->toString();

    this->consoleList->insert(ip, {ip, name, model, version});
}
