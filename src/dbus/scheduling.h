#ifndef DBUSSCHEDULING_H
#define DBUSSCHEDULING_H

#include <QObject>
#include <QDBusContext>

class DBusScheduling : public QObject, protected QDBusContext
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.halium.mechanicd.Scheduling")

public:
    DBusScheduling(QObject *parent = nullptr);
    ~DBusScheduling();

public Q_SLOTS:
    void requestSchedulingChange();

Q_SIGNALS:
    void schedulingRequest(int processId);
};

#endif