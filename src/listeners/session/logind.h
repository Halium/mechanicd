#ifndef LOGINDLISTENER_H
#define LOGINDLISTENER_H

#include "interface.h"

class LogindListener : public QObject, public SessionInterface
{
    Q_OBJECT
    Q_INTERFACES(SessionInterface)

public:
    LogindListener(QObject* parent = nullptr);

Q_SIGNALS:
    void userLoggedIn(uid_t userId) override;
    void userLoggedOut(uid_t userId) override;
};

#endif