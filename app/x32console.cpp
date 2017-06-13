#include "x32console.h"

X32Console::X32Console(QObject *parent) : QObject(parent)
{
    this->dataPool = new QVector<OscMessage*>();
    this->mutegroups = new QMap<qint16, Mutegroup*>();
    this->channels = new QMap<qint16, Channel*>();

    for(int i=0; i<8; i++) {
        Mutegroup *mg = new Mutegroup(this, i, false, this);
        connect(this, SIGNAL(distributeMessage(QString,OscMessage&)), mg, SLOT(findMessage(QString,OscMessage&)));
        this->mutegroups->insert(i, mg);
    }

    for(int i=0; i<32; i++) {
        Channel *chan = new Channel(this, this);
        connect(this, SIGNAL(distributeMessage(QString,OscMessage&)), chan, SLOT(findMessage(QString,OscMessage&)));
        this->channels->insert(i, chan);
    }

}

void X32Console::setSocket(OscUdpSocket *socket)
{
    this->socket = socket;
}

void X32Console::sendMessage(OscMessageComposer msg)
{
    if(this->socket == nullptr) return;
    socket->sendData(msg.getBytes());
}

void X32Console::removeMessage(OscMessage &msg)
{
    this->dataPool->removeOne(msg);
}

void X32Console::handleMessage(QNetworkDatagram data)
{
    qDebug() << "Data: " << data.data();

    QByteArray byteData(data.data());

    OscReader reader(&byteData);
    OscMessage* msg = reader.getMessage();

    QString address = msg->getAddress();	// Get the message address
    qDebug() << address << "#" << msg->getNumValues();

    // remove from pool : this->dataPool->removeOne()
    this->dataPool->append(msg);

    emit distributeMessage(msg->getAddress(), *msg);
}
