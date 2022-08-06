#ifndef UPSTARTLISTENER_H
#define UPSTARTLISTENER_H

#include "interface.h"

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusObjectPath>
#include <QList>
#include <QVariant>

class UpstartListener : public QObject, public ServiceInterface
{
    Q_OBJECT
    Q_INTERFACES(ServiceInterface)

public:
    UpstartListener(QObject* parent = nullptr);
    ~UpstartListener();

    virtual QList<pid_t> getPidsForServiceName(const QString& name) override;

private Q_SLOTS:
    void inspectJob(QString eventType, QStringList args);

private:
    QDBusConnection m_upstartConnection;

Q_SIGNALS:
    virtual void serviceStarted(const QString name) override;
};

#endif