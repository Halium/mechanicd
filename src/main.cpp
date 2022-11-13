#include <QCoreApplication>
#include <QDBusConnection>
#include <QDebug>
#include <QFile>
#include <QThread>

#include <sched.h>

#include "listeners/power.h"
#include "listeners/session.h"
#include "listeners/service.h"
#include "tuners/renderthread.h"
#include "tuners/schedtune.h"
#include "dbus/scheduling.h"
#include "dbus/wifimodeswitch.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    if (!QDBusConnection::systemBus().registerService("org.halium.mechanicd")) {
        qCritical() << "Failed to register DBus service.";
        exit(1);
    }

#if 0
    PowerListener power;
    SessionListener session;
    ServiceListener services;
#endif

    SchedTuneTuner schedTuneTuner;
    RenderThreadTuner renderThreadTuner;
    DBusScheduling dbusScheduling;
    DBusWifiModeSwitch dbusWifiModeSwitch;

    if (!QDBusConnection::systemBus().registerObject("/org/halium/mechanicd/Scheduling",
                                                      &dbusScheduling,
                                                      QDBusConnection::ExportAllSlots)) {
        qCritical() << "Failed to register scheduling object";
        exit(1);
    }

    if (!QDBusConnection::systemBus().registerObject("/org/halium/mechanicd/WifiMode",
                                                      &dbusWifiModeSwitch,
                                                      QDBusConnection::ExportAllSlots)) {
        qCritical() << "Failed to register wifi mode object";
        exit(1);
    }

    // Allow shells and apps to request FIFO scheduling
    // Android does this through ActivityManager, we do it over DBus requests.
    // This is similar to RealtimeKit, but allows setting arbitrary thread's priorities
    // and it seems kind of dead nowadays. Also only allows SCHED_RR instead of SCHED_FIFO.
    // haliumqsgcontext is a notable consumer which gets Android-like scheduling for free.
    QObject::connect(&dbusScheduling, &DBusScheduling::schedulingRequest,
                     &app, [&renderThreadTuner, &schedTuneTuner](int processId) {
        renderThreadTuner.tunePid(processId);
        schedTuneTuner.tunePid(processId);
    }, Qt::QueuedConnection);

    return app.exec();
}
