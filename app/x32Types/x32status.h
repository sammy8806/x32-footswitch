#ifndef X32STATUS_H
#define X32STATUS_H

#include <QObject>
#include "x32Types/x32type.h"

#include <osc/reader/OscMessage.h>
#include <osc/reader/types/OscString.h>
#include <osc/reader/types/OscValue.h>

class X32Status
{

public:
    explicit X32Status (OscMessage *message);

    QString unknown;
    QString consoleName;
    QString consoleType;
    QString consoleVersion;

    static QRegExp typeRegExp() {
        return QRegExp("^/([x]{,1}status|info).*");
    }

signals:

public slots:
};

#endif // X32STATUS_H
