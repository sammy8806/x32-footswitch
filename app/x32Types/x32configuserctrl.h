#ifndef X32CONFIGUSERCTRL_H
#define X32CONFIGUSERCTRL_H

#include <QObject>
#include <QDebug>
#include <QRegExp>
#include <assert.h>
#include <qmath.h>

#include <x32Types/x32type.h>

#include <osc/reader/OscMessage.h>
#include <osc/reader/types/OscValue.h>
#include <osc/reader/types/OscString.h>
#include <osc/reader/types/OscFloat.h>
#include <osc/reader/types/OscInteger.h>

class X32ConfigUserctrl : public QObject
{
    Q_OBJECT

public:
    explicit X32ConfigUserctrl(X32ConsoleAbstract *console, QObject *parent = nullptr);

    static QRegExp typeRegExp() {
        return QRegExp("^/config/userctrl.*");
    }

    void parseUserCtrl(QString data);
    QMap<QChar, UserctrlBank *>* getBanks();

    void refreshBank(QChar bank);

    UserctrlButton buttonData;
    QString page;
    QString number;

    QMap<QString, X32ConfigBtn> *btnMap;
    QMap<QString, X32ConfigRotary> *rotaryMap;

private:
    QMap<QChar, UserctrlBank*> *assignData;
    X32ConsoleAbstract *console;

    void updateBtn(UserctrlBank* bank, qint8 btnNr, OscMessage& data);
    void updateBank(UserctrlBank* bank, qint8 color);

public:
signals:
    void updatedButton(UserctrlBank *bank, qint8 btnNr);
    void updatedBank(UserctrlBank *bank);

public slots:
    X32_INTERNAL void findMessage(QString address, OscMessage& data) {
        if(address.left(16) != "/config/userctrl") return;

        QChar targetBank = address.at(17);
        UserctrlBank *bank = assignData->value(targetBank);

        if(address.mid(19, 3) == "btn") {
            qint8 btnNr = address.mid(23, 2).toInt();
            assert(btnNr != 0);
            updateBtn(bank, btnNr, data);
        }

        if(address.mid(19, 5) == "color") {
            qint8 color = data.getValue(0)->toInteger();
            assert(color >= 0);
            updateBank(bank, color);
        }

        this->console->removeMessage(data);
    }

};

#endif // X32CONFIGUSERCTRL_H
