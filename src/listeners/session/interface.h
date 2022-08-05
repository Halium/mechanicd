#ifndef SESSIONINTERFACE_H
#define SESSIONINTERFACE_H

#include <QObject>

class SessionInterface
{
public:
    virtual ~SessionInterface() {}

Q_SIGNALS:
};
Q_DECLARE_INTERFACE(SessionInterface, "SessionInterface")

#endif