#ifndef SERVICELISTENER_H
#define SERVICELISTENER_H

#include <QObject>

#include "service/interface.h"

class ServiceListener : public QObject, public ServiceInterface
{
    Q_OBJECT
    Q_INTERFACES(ServiceInterface)

public:
    ServiceListener(QObject* parent = nullptr);

    virtual QList<pid_t> getPidsForServiceName(const QString& name) override;

private:
    ServiceInterface* m_impl;

Q_SIGNALS:
    virtual void serviceStarted(const QString name) override;
};

#endif