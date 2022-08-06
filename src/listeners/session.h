#ifndef SESSIONLISTENER_H
#define SESSIONLISTENER_H

#include "session/interface.h"

class SessionListener : public QObject, public SessionInterface
{
    Q_OBJECT
    Q_INTERFACES(SessionInterface)

public:
    SessionListener(QObject* parent = nullptr);

private:
    SessionInterface* m_impl;

Q_SIGNALS:
    void userLoggedIn(uid_t userId) override;
    void userLoggedOut(uid_t userId) override;
};

#endif