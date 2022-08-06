#include "service.h"

#include "service/upstart.h"

ServiceListener::ServiceListener(QObject* parent) : QObject(parent),
    m_impl(new UpstartListener(this))
{
    QObject::connect((UpstartListener*)m_impl, &UpstartListener::serviceStarted, this, &ServiceListener::serviceStarted);
}

QList<pid_t> ServiceListener::getPidsForServiceName(const QString& name)
{
    return m_impl->getPidsForServiceName(name);
}