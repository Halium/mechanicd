#include "scheduling.h"

#include <QDBusConnectionInterface>

DBusScheduling::DBusScheduling(QObject *parent) : QObject(parent)
{
}

DBusScheduling::~DBusScheduling()
{
}

void DBusScheduling::requestSchedulingChange()
{
    const auto service = message().service();
    const auto pid = connection().interface()->servicePid(service); 
    Q_EMIT schedulingRequest(pid);
}
