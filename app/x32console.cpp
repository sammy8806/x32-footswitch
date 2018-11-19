#include "x32console.h"

X32Console::X32Console(QHostAddress consoleAddress, int consolePort, QObject *parent) : X32ConsoleAbstract(parent)
{
    this->consoleAddress = consoleAddress;
    this->consolePort = consolePort;

    this->dataPool = new QVector<OscMessage*>();
    this->mutegroups = new QMap<quint8, Mutegroup*>();
    this->channels = new QMap<quint8, Channel*>();
    this->dcas = new QMap<quint8, Dca*>();

    this->config = new X32Config(this, this);
    connect(config->userctrl, SIGNAL(updatedButton(UserctrlBank*,qint8)), this, SLOT(updatedUserctrlButton(UserctrlBank*,qint8)));
    connect(config->userctrl, SIGNAL(updatedBank(UserctrlBank*)), this, SLOT(updatedUserctrlBank(UserctrlBank*)));

    for(quint8 i=1; i<=MAX_DCA; i++) {
        Dca *dca = new Dca(this, i, false, this);
        connect(this, SIGNAL(distributeMessage(QString,OscMessage&)), dca, SLOT(findMessage(QString,OscMessage&)));
        this->dcas->insert(i, dca);
    }

    for(quint8 i=1; i<=MAX_MUTEGROUP; i++) {
        Mutegroup *mg = new Mutegroup(this, i, false, this);
        connect(this, SIGNAL(distributeMessage(QString,OscMessage&)), mg, SLOT(findMessage(QString,OscMessage&)));
        this->mutegroups->insert(i, mg);
    }

    for(quint8 i=1; i<=MAX_CHAN; i++) {
        Channel *chan = new Channel(this, i, this);
        connect(this, SIGNAL(distributeMessage(QString,OscMessage&)), chan, SLOT(findMessage(QString,OscMessage&)));
        connect(chan, SIGNAL(updated(Channel*)), this, SLOT(updatedChannel(Channel*)));
        this->channels->insert(i, chan);
    }

    connect(this, SIGNAL(distributeMessage(QString,OscMessage&)), config, SLOT(findMessage(QString,OscMessage&)));
    connect(this, SIGNAL(distributeMessage(QString,OscMessage&)), this, SLOT(handleNode(QString,OscMessage&)));
}

void X32Console::enableConsole()
{
    DebugLog << "Enabling Console";

    BackgroundHeartbeat *heartbeat = new BackgroundHeartbeat(this);
    QObject::connect(heartbeat, SIGNAL(sendMessage(OscMessageComposer)), this, SLOT(sendMessage(OscMessageComposer)));
    this->refreshValues();
}

QString X32Console::parseButtonData(QString data, X32Console* console)
{
    QString output;

    if(data.length() == 0) return output;

    if(data.at(0) == 'O') {
        output = "Mute";

        output.append(X32Console::parseChannelName(static_cast<quint8>(data.right(2).toInt()), console));
    } else {
        output = data;
    }

    return output;
}

QString X32Console::parseChannelName(quint8 channelNumber, X32Console* console)
{
    QString output = "";

    // TODO: Return Channel Names if set

    if(channelNumber <= CHAN_NORMAL_MAX) {
        Channel* chan = nullptr;
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
        output = "FX " + QString::number(static_cast<int>(ceil((channelNumber - CHAN_AUX_MAX + 1)/2))) + QString(left ? "L" : "R");
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
    DebugLog << "Refreshing Values";

    for(quint8 i=1; i<=MAX_CHAN; i++)
        this->channels->value(i)->refresh();

    for(quint8 i=1; i<=MAX_MUTEGROUP; i++)
        this->mutegroups->value(i)->refresh();
}

QString X32Console::getConsoleName()
{
    return this->consoleAddress.toString().append(QString(this->consolePort));
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

void X32Console::handleNode(QString address, OscMessage& dataPtr)
{
    if(address.left(5) != "node\0x00") return;

    QString data = dataPtr.getValue(0)->toString();

    int i = 0;
    QChar activeChar;

    bool inAddress = true;
    QString addr = "";

    for(; i<data.length() && inAddress; i++) {
        activeChar = data.at(i);

        if(inAddress && activeChar == ' ') {
            inAddress = false;
        }
        if(inAddress) {
            addr.append(activeChar);
        }
    }

    OscMessageComposer composer = OscMessageComposer(addr);

    bool hasData = false;
    bool inString = false;
    QString tmp = "";

    bool formatOk = false;
    float bufFloat;
    int bufInt;

    for(; i<data.length(); i++) {
        activeChar = data.at(i);

        if(activeChar == '"' && !inString) {
            inString = true;
            hasData = true;
        } else if(activeChar == ' ' && !inString) {
TYPE_GUESS:
            if(tmp == "OFF" || tmp == "ON") { // Bool
                composer.pushInt32( tmp == "ON" );
            } else if(tmp.contains('.')) {
                bufFloat = tmp.toFloat(&formatOk);
                if(!formatOk) goto PUSH_STRING;
                composer.pushFloat(bufFloat);
                bufFloat = 0.0f;
                formatOk = false;
            } else {
                bufInt = tmp.toInt(&formatOk);
                if(formatOk) {
                    composer.pushInt32(bufInt);
                }
PUSH_STRING:
                composer.pushString(tmp);
            }

            tmp.clear();
            hasData = false;
        } else if(activeChar == '"' && inString) {
            inString = false;
        } else if(activeChar != 0xA) {
            hasData = true;
            tmp.append(activeChar);
        }
    }

    if(tmp.length() != 0 || ( hasData && tmp.length() == 0 )) {
        goto TYPE_GUESS;
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

void X32Console::updatedUserctrlBank(UserctrlBank *bank)
{
    qDebug() << "[Console] Updated UserCtrl Bank " << bank->bank << " with color " << bank->color;
    emit updateUserctrlBank(bank);
}

void X32Console::mute(qint8 chan)
{
    bool status = false;

    if(chan <= CHAN_NORMAL_MAX)
        status = !channels->value(chan+1)->mix.on;

    if(chan >= CHAN_DCA_MIN && chan <= CHAN_DCA_MAX)
        status = !dcas->value(chan - CHAN_MAINMC)->active;

    if(chan >= CHAN_MUTEGROUP_MIN && chan <= CHAN_MUTEGROUP_MAX)
        status = !mutegroups->value(chan - CHAN_DCA_MAX)->state;

    qDebug() << "[Console] " << (status ? "" : "Un") << "Mute " << chan;

    if(chan <= CHAN_NORMAL_MAX)
        channels->value(chan+1)->mute(status);

    if(chan >= CHAN_DCA_MIN && chan <= CHAN_DCA_MAX)
        dcas->value(chan - CHAN_MAINMC)->mute(status);

    if(chan >= CHAN_MUTEGROUP_MIN && chan <= CHAN_MUTEGROUP_MAX)
        mutegroups->value(chan - CHAN_DCA_MAX)->mute(status);
}

void X32Console::recall(QString target)
{
    qint8 type = target.left(1).toInt();
    qint8 toLoad = target.right(2).toInt();

    switch (type) {
    case 0: // Scene
    {
        OscMessageComposer msgScene("/-action/goscene");
        msgScene.pushInt32(toLoad);
        socket->sendData(msgScene.getBytes());
    }
        break;

    case 2: // Snippet
    {
        OscMessageComposer msgSnippet("/-action/gosnippet");
        msgSnippet.pushInt32(toLoad);
        socket->sendData(msgSnippet.getBytes());
    }
        break;

    case 4: // Cue
    {
        OscMessageComposer msgCue("/-action/gocue");
        msgCue.pushInt32(toLoad);
        socket->sendData(msgCue.getBytes());
    }
        break;
    }
}

QString X32Console::getLogPrefix() {
    return this->consoleAddress.toString() + QString(this->consolePort);
}
