#include "x32status.h"

X32Status::X32Status(OscMessage *message)
{
    this->unknown = message->getValue(0)->toString();
    this->consoleName = message->getValue(1)->toString();
    this->consoleType = message->getValue(2)->toString();
    this->consoleVersion = message->getValue(3)->toString();
}
