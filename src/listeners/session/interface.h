#ifndef SESSIONINTERFACE_H
#define SESSIONINTERFACE_H

#include <QObject>

class SessionInterface
{
public:
    SessionInterface() = default;
    virtual ~SessionInterface() {}

Q_SIGNALS:
    virtual void userLoggedIn(uid_t userId) = 0;
    virtual void userLoggedOut(uid_t userId) = 0;
};
Q_DECLARE_INTERFACE(SessionInterface, "SessionInterface")

#endif