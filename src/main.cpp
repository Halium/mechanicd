#include <QCoreApplication>
#include <QDBusConnection>
#include <QDebug>
#include <QFile>
#include <QThread>

#include <sched.h>

#include "listeners/power.h"
#include "listeners/session.h"
#include "listeners/service.h"
#include "tuners/schedtune.h"
#include "dbus/scheduling.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    if (!QDBusConnection::systemBus().registerService("org.halium.mechanicd")) {
        qCritical() << "Failed to register DBus service.";
        exit(1);
    }

    PowerListener power;
    SessionListener session;
    ServiceListener services;
    SchedTuneTuner schedTuneTuner;
    DBusScheduling dbusScheduling;

    if (!QDBusConnection::systemBus().registerObject("/org/halium/mechanicd/Scheduling",
                                                      &dbusScheduling,
                                                      QDBusConnection::ExportAllSlots)) {
        qCritical() << "Failed to register scheduling object";
        exit(1);
    }

    // Allow shells and apps to request FIFO scheduling
    // Android does this through ActivityManager, we do it over DBus requests.
    // This is similar to RealtimeKit, but it seems kind of dead nowadays
    // and only allows SCHED_RR instead of SCHED_FIFO.
    QObject::connect(&dbusScheduling, &DBusScheduling::schedulingRequest,
                     &app, [](int processId, int threadId){
        const QString threadComm = QStringLiteral("/proc/%1/task/%2/comm").arg(QString::number(processId),
                                                                               QString::number(threadId));
        qDebug() << "Scheduling request for" << threadComm;

        QFile threadCommFile(threadComm);
        if (!threadCommFile.exists()) {
            qWarning() << threadComm << "doesn't exist. Spoofing attempt?";
            return;
        }

        if (!threadCommFile.open(QFile::ReadOnly)) {
            qWarning() << "Unable to open" << threadComm;
            return;
        }

        const QString threadName = QString::fromUtf8(threadCommFile.readAll());
        if (threadName.trimmed() != QStringLiteral("QSGRenderThread")) {
            return;
        }

        struct sched_param param = { .sched_priority = 50 };
        sched_setscheduler(threadId, SCHED_FIFO | SCHED_RESET_ON_FORK, &param);
    }, Qt::QueuedConnection);

    return app.exec();
}
