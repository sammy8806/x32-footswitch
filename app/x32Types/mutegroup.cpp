#include "mutegroup.h"

Mutegroup::Mutegroup(X32ConsoleAbstract *console, qint16 number, bool state, QObject *parent) : QObject(parent),
    console(console), number(number), state(state)
{

}
