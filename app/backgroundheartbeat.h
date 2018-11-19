#ifndef BACKGROUNDHEARTBEAT_H
#define BACKGROUNDHEARTBEAT_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QHostAddress>

#include <osc/composer/OscMessageComposer.h>

class BackgroundHeartbeat : public QObject
{
    Q_OBJECT
public:
    explicit BackgroundHeartbeat(QObject *parent = nullptr);

private:
    QTimer* timer;

signals:
    void sendMessage(OscMessageComposer data);

public slots:
    void tick();
};

#endif // BACKGROUNDHEARTBEAT_H
