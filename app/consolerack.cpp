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
    if(this->consoles->size() == 0) {
        return nullptr;
    }

    return this->consoles->first();
}

void ConsoleRack::registerConsole(QHostAddress address, int port, XInfo info)
{
    QRegularExpression matchX32("^[MX]32(?:RACK|C|P|R)*$");

    if(!matchX32.match(info.model).hasMatch()) {
        qDebug() << "Found non X32 Console => ignoring it :" << info.model;
        return;
    } else {
        qDebug() << "Registering" << info.name;
    }

    X32Console *console = new X32Console(address, port, this);
    console->setSocket(this->socket);
    console->enableConsole();

    qDebug() << "! Enabled" << info.name << "!";

    this->consoles->insert(info.ip, console);
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

    if(ip == "") {
        ip = data.senderAddress().toString();
    }

    qDebug() << "Found " << model << " " << name << " (" << version << ") [" << ip << "]";

    if(this->getConsole(ip) != nullptr) {
        this->registerConsole(data.senderAddress(), data.senderPort(), {ip, name, model, version});
    } else {
        qDebug() << "Console" << name << "already registered ... skipping";
    }
}
