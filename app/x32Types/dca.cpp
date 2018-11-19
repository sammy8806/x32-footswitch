#include "dca.h"

Dca::Dca(X32ConsoleAbstract *console, quint8 number, bool state, QObject *parent) : QObject(parent),
    console(console), refreshTimer(new QTimer()), number(number), active(state)
{
    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(timedRefresh()));
    this->refreshTimer->setSingleShot(true);
}
