#include "mutegroup.h"

Mutegroup::Mutegroup(X32ConsoleAbstract *console, qint16 number, bool state, QObject *parent) : QObject(parent),
    console(console), refreshTimer(new QTimer()), number(number), state(state)
{
    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(timedRefresh()));
    this->refreshTimer->setSingleShot(true);
}
