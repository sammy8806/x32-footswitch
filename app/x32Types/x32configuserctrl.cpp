#include "x32configuserctrl.h"

X32ConfigUserctrl::X32ConfigUserctrl(OscMessage *msg)
{
    QRegExp configFilter("^/config/userctrl/([a-zA-Z])/btn/([0-9]+)"); // "^([a-zA-Z])(.*)"
    configFilter.indexIn(msg->getAddress());
    QStringList lst = configFilter.capturedTexts();

    // is a button
    if(lst.length() == 3) {
        QString page = lst.at(1);
        QString btn = lst.at(2);

        qDebug() << "ConfigData: " << page << btn;

        this->page = page;
        this->number = btn;

        QString data = msg->getValue(0)->toString();

        if(data.length() > 0) {
            this->buttonData.data = data;
        }
    }
}
