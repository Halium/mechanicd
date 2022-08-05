#ifndef SESSIONLISTENER_H
#define SESSIONLISTENER_H

#include "session/interface.h"

class SessionListener : public QObject, public SessionInterface
{
    Q_OBJECT
    Q_INTERFACES(SessionInterface)

public:
    SessionListener(QObject* parent = nullptr);
};

#endif