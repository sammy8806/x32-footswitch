#include "backgroundheartbeat.h"

BackgroundHeartbeat::BackgroundHeartbeat(QObject *parent) : QObject(parent), timer(new QTimer(this))
{
    connect(this->timer, SIGNAL(timeout()), this, SLOT(tick()));

    this->timer->start(7000);
}

void BackgroundHeartbeat::tick()
{
    qDebug() << "Renewing /xremote";
    OscMessageComposer msg("/xremote");
    emit sendMessage(msg);
}
