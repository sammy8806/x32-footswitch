#include "x32console.h"

X32Console::X32Console(QObject *parent) : X32ConsoleAbstract(parent)
{
    this->dataPool = new QVector<OscMessage*>();
    this->mutegroups = new QMap<qint16, Mutegroup*>();
    this->channels = new QMap<qint16, Channel*>();

    this->config = new X32Config(this, this);
    connect(config->userctrl, SIGNAL(updatedButton(UserctrlBank*,qint8)), this, SLOT(updatedUserctrlButton(UserctrlBank*,qint8)));

    for(int i=0; i<8; i++) {
        Mutegroup *mg = new Mutegroup(this, i, false, this);
        connect(this, SIGNAL(distributeMessage(QString,OscMessage&)), mg, SLOT(findMessage(QString,OscMessage&)));
        this->mutegroups->insert(i, mg);
    }

    for(int i=1; i<=32; i++) {
        Channel *chan = new Channel(this, i, this);
        connect(this, SIGNAL(distributeMessage(QString,OscMessage&)), chan, SLOT(findMessage(QString,OscMessage&)));
        connect(chan, SIGNAL(updated(Channel*)), this, SLOT(updatedChannel(Channel*)));
        this->channels->insert(i, chan);
    }

    connect(this, SIGNAL(distributeMessage(QString,OscMessage&)), config, SLOT(findMessage(QString,OscMessage&)));
    connect(this, SIGNAL(distributeMessage(QString,OscMessage&)), this, SLOT(handleNode(QString,OscMessage&)));
}

QString X32Console::parseButtonData(QString data, X32Console* console)
{
    QString output;

    if(data.length() == 0) return output;

    if(data.at(0) == 'O') {
        output = "Mute";

        output.append(X32Console::parseChannelName(data.right(2).toInt(), console));
    } else {
        output = data;
    }

    return output;
}

QString X32Console::parseChannelName(qint8 channelNumber, X32Console* console)
{
    QString output = "";

    // TODO: Return Channel Names if set

    if(channelNumber <= CHAN_NORMAL_MAX) {
        Channel* chan;
        if(console != nullptr)
            chan = console->channels->value(channelNumber+1);

        if(chan != nullptr && chan->config.name != "")
            output = chan->config.name;
        else
            output = "Channel " + QString::number(channelNumber+1);

    } else if (channelNumber <= CHAN_AUX_MAX) {
        output = "Aux " + QString::number(channelNumber - CHAN_NORMAL_MAX);
    } else if (channelNumber <= CHAN_FX_MAX) {
        bool left = (channelNumber - CHAN_AUX_MAX) % 2;
        output = "FX " + QString::number((int)ceil((channelNumber - CHAN_AUX_MAX + 1)/2)) + QString(left ? "L" : "R");
    } else if (channelNumber <= CHAN_BUS_MAX) {
        output = "MixBus " + QString::number(channelNumber - CHAN_FX_MAX);
    } else if (channelNumber <= CHAN_MATRIX_MAX) {
        output = "Matrix " + QString::number(channelNumber - CHAN_BUS_MAX);
    } else if (channelNumber <= CHAN_MAINLR) {
        output = "Main LR";
    } else if (channelNumber <= CHAN_MAINMC) {
        output = "Main M/C";
    } else if (channelNumber <= CHAN_DCA_MAX) {
        output = "DCA " + QString::number(channelNumber - CHAN_MAINMC);
    } else if (channelNumber <= CHAN_MUTEGROUP_MAX) {
        output = "MuteGroup " + QString::number(channelNumber - CHAN_DCA_MAX);
    }

    return output;
}

void X32Console::refreshValues()
{
/*    for(int i=1; i<=32; i++)
        this->channels->value(i)->refresh();
*/
    for(int i=0; i<8; i++)
        this->mutegroups->value(i)->refresh();
}

void X32ConsoleAbstract::setSocket(OscUdpSocket *socket)
{
    this->socket = socket;
}

void X32ConsoleAbstract::sendMessage(OscMessageComposer msg)
{
    if(this->socket == nullptr) return;
    socket->sendData(msg.getBytes());
}

void X32ConsoleAbstract::removeMessage(OscMessage &msg)
{
    this->dataPool->removeOne(&msg);
}

void X32Console::handleMessage(QNetworkDatagram data)
{
    // qDebug() << "Data: " << data.data();

    QByteArray byteData(data.data());

    // TODO: Meter Handling

    OscReader reader(&byteData);
    OscMessage* msg = reader.getMessage();

    QString address = msg->getAddress();	// Get the message address
    // qDebug() << address << "#" << msg->getNumValues();

    // remove from pool : this->dataPool->removeOne()
    this->dataPool->append(msg);

    emit distributeMessage(msg->getAddress(), *msg);
}

// TODO: Let's get this working properly
void X32Console::handleNode(QString address, OscMessage& dataPtr)
{
    if(address.left(5) != "node\0x00") return;

    QString data = dataPtr.getValue(0)->toString();

    QStringList args = data.split(QChar(' '));
    QString addr = args.at(0);
    QString value = "";
    for(int i=1; i<=args.length()-1; i++) {
        value.append(args.at(i) + " ");
    }

    this->removeMessage(dataPtr);
    value.remove(0xA);
    value = value.trimmed();

    OscMessageComposer composer = OscMessageComposer(addr);

    bool hasData = false;
    QString tmp = "";
    bool inString = false;
    QChar activeChar;

    for(int i=0; i<value.length(); i++) {
        activeChar = value.at(i);

        if(activeChar == '"' && !inString) {
            inString = true;
            hasData = true;
        } else if(activeChar == ' ' && !inString) {
            composer.pushString(tmp);
            tmp.clear();
            hasData = false;
        } else if(activeChar == '"' && inString) {
            inString = false;
        } else {
            hasData = true;
            tmp.append(activeChar);
        }
    }

    if(tmp.length() != 0 || ( hasData && tmp.length() == 0 )) {
        composer.pushString(tmp);
        tmp.clear();
    }

    // qDebug() << "Handling node-answer for " << addr;

    // composer.pushString(value.trimmed());

    // qDebug() << "Value: " << value;

    OscReader reader(composer.getBytes());
    OscMessage* msg = reader.getMessage();

    emit distributeMessage(addr, *msg);
}

void X32Console::updatedChannel(Channel *channel)
{
    qDebug() << "[Console] Updated Chan #" << channel->getNumber();

    emit updateChannel(channel);
}

void X32Console::updatedUserctrlButton(UserctrlBank *bank, qint8 btnNr)
{
    qDebug() << "[Console] Updated UserCtrl Button # " << bank->bank << btnNr << " to " << bank->data->value(btnNr).data;
    emit updateUserctrlButton(bank, btnNr);
}

void X32Console::mute(qint8 chan)
{
    bool status;

    if(chan <= 31)
        status = !channels->value(chan+1)->mix.on;

    qDebug() << "[Console] " << (status ? "" : "Un") << "Mute " << chan;

    if(chan <= 31)
        channels->value(chan+1)->mute(status);
}
