#ifndef X32CONSOLE_H
#define X32CONSOLE_H

#include <QObject>
#include <oscudpsocket.h>
#include "backgroundheartbeat.h"

#include <osc/composer/OscMessageComposer.h>

#include "x32Types/x32type.h"
#include "x32Types/channel.h"
#include "x32Types/mutegroup.h"
#include "x32Types/dca.h"
#include "x32Types/x32configuserctrl.h"
#include "x32Types/config.h"

class X32Console : public X32ConsoleAbstract
{
    Q_OBJECT
public:
    explicit X32Console(QHostAddress consoleAddress, int port = 10023, QObject *parent = nullptr);

    void enableConsole();
    void refreshValues();

    static QString parseButtonData(QString data, X32Console *console = nullptr);
    static QString parseChannelName(quint8 channelNumber, X32Console *console = nullptr);

    QMap<quint8, Mutegroup*> *mutegroups;
    QMap<quint8, Channel*> *channels;
    QMap<quint8, Dca*> *dcas;
    X32Config *config;

    // QMap<qint16, Cue> cues;
    // QMap<qint16, Scene> scenes;

protected:
    X32_INTERNAL QString getLogPrefix() override;

signals:
    X32_INTERNAL void distributeMessage(QString address, OscMessage& dataPtr);
    void updateChannel(Channel* channel);
    void updateUserctrlButton(UserctrlBank *bank, qint8 btnNr);
    void updateUserctrlBank(UserctrlBank* bank);

public slots:
    void handleMessage(QNetworkDatagram data);
    void handleNode(QString address, OscMessage& dataPtr);
    void updatedChannel(Channel* channel);
    void updatedUserctrlButton(UserctrlBank *bank, qint8 btnNr);
    void updatedUserctrlBank(UserctrlBank* bank);

    void mute(qint8 chan);
    void recall(QString target);

    X32_INTERNAL virtual void sendMessage(OscMessageComposer msg) override {
        if(this->socket == nullptr) return;
        socket->sendData(msg.getBytes(), this->consoleAddress, this->consolePort);
    }

};

#endif // X32CONSOLE_H
