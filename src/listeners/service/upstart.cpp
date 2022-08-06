#include "upstart.h"

#include <QDebug>
#include <QDBusArgument>
#include <QDBusReply>
#include <QDBusMetaType>

static const QString UPSTART_SERVICE = QStringLiteral("com.ubuntu.Upstart");
static const QString UPSTART_PATH = QStringLiteral("/com/ubuntu/Upstart");
static const QString UPSTART_INTERFACE = QStringLiteral("com.ubuntu.Upstart0_6");
static const QString UPSTART_JOBS_INTERFACE = QStringLiteral("com.ubuntu.Upstart0_6.Job");
static const QString UPSTART_INSTANCE_INTERFACE = QStringLiteral("com.ubuntu.Upstart0_6.Instance");

struct UpstartInstance
{
    QString name;
    int pid;
};
Q_DECLARE_METATYPE(UpstartInstance);

QDBusArgument &operator<<(QDBusArgument &argument, const UpstartInstance &myStruct)
{
    argument.beginStructure();
    argument << myStruct.name << myStruct.pid;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, UpstartInstance &myStruct)
{
    argument.beginStructure();
    argument >> myStruct.name >> myStruct.pid;
    argument.endStructure();
    return argument;
}

UpstartListener::UpstartListener(QObject* parent) : QObject(parent),
    m_upstartConnection(QDBusConnection::connectToBus(QDBusConnection::SystemBus, "mechanicd"))
{
    qRegisterMetaType<UpstartInstance>("UpstartInstance");
    qDBusRegisterMetaType<UpstartInstance>();

    qDebug() << m_upstartConnection.connect(
        UPSTART_SERVICE,
        UPSTART_PATH,
        UPSTART_INTERFACE,
        "EventEmitted",
        this,
        SLOT(inspectJob(QString, QStringList)));
}

UpstartListener::~UpstartListener()
{
}

void UpstartListener::inspectJob(QString eventType, QStringList args)
{
    QString jobName;

    if (eventType != QString("started"))
        return;

    const QString prefix = QStringLiteral("JOB=");
    for (const QString& arg : args) {
        if (!arg.startsWith(prefix))
            continue;

        jobName = arg.mid(prefix.length());
    }

    if (!jobName.isEmpty()) {
        qDebug() << "Service started:" << jobName;
        Q_EMIT serviceStarted(jobName);
    }
}

QList<pid_t> UpstartListener::getPidsForServiceName(const QString& name)
{
    QList<pid_t> pids;

    QDBusInterface manager(UPSTART_SERVICE,
                           UPSTART_PATH,
                           UPSTART_INTERFACE,
                           m_upstartConnection);
    QDBusReply<QDBusObjectPath> reply = manager.call("GetJobByName", name);
    if (!reply.isValid()) {
        qWarning() << "Failed to get job by name";
        return pids;
    }

    QDBusObjectPath path = reply.value();
    QDBusInterface job(UPSTART_SERVICE,
                       path.path(),
                       UPSTART_JOBS_INTERFACE,
                       m_upstartConnection);
    QDBusReply<QList<QDBusObjectPath> > jobReply = job.call("GetAllInstances");
    if (!jobReply.isValid()) {
        qWarning() << "Failed to get all instances at job path" << path.path();
        return pids;
    }

    QList<QDBusObjectPath> instances = jobReply.value();

    for (const QDBusObjectPath& instancePath : instances) {
        QDBusInterface instance(UPSTART_SERVICE,
                                instancePath.path(),
                                "org.freedesktop.DBus.Properties",
                                m_upstartConnection);

        auto property = instance.call("Get", UPSTART_INSTANCE_INTERFACE, "processes");
        auto returnVariant = property.arguments().at(0);

        QDBusVariant propertyDbusVariant = returnVariant.value<QDBusVariant>();
        QDBusArgument args = propertyDbusVariant.variant().value<QDBusArgument>();

        UpstartInstance upstartInstance;
        args.beginArray();
        while (!args.atEnd()) {
            args >> upstartInstance;
            qDebug() << upstartInstance.pid;
            pids << upstartInstance.pid;
        }
        args.endArray();
    }

    return pids;
}