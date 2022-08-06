#ifndef SERVICEINTERFACE_H
#define SERVICEINTERFACE_H

#include <QObject>
#include <QList>

class ServiceInterface
{
public:
    ServiceInterface() = default;
    virtual ~ServiceInterface() {}

    virtual QList<pid_t> getPidsForServiceName(const QString& name) = 0;

Q_SIGNALS:
    virtual void serviceStarted(const QString name) = 0;
};
Q_DECLARE_INTERFACE(ServiceInterface, "ServiceInterface")

#endif