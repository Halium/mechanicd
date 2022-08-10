#include "renderthread.h"

#include <QDir>
#include <QDirIterator>
#include <QDebug>

RenderThreadTuner::RenderThreadTuner(QObject* parent) : QObject(parent)
{
    
}

void RenderThreadTuner::tunePid(pid_t pid)
{
    const QString pidDir = QStringLiteral("/proc/%1/task").arg(QString::number(pid));
    QDirIterator it(pidDir, QDir::Dirs | QDir::NoDotAndDotDot);

    QStringList eligibleThreads;

    while (it.hasNext()) {
        const QString candidate = it.next();
        const QStringList candidateParts = candidate.split("/", QString::SkipEmptyParts);
        if (candidateParts.length() != 4)
            continue;

        const QString threadId = candidateParts[3];
        const QString threadComm = QStringLiteral("/proc/%1/task/%2/comm").arg(QString::number(pid), threadId);

        QFile threadCommFile(threadComm);
        if (!threadCommFile.exists()) {
            qWarning() << threadComm << "doesn't exist. Spoofing attempt?";
            continue;
        }

        if (!threadCommFile.open(QFile::ReadOnly)) {
            qWarning() << "Unable to open" << threadComm;
            continue;
        }

        const QString threadName = QString::fromUtf8(threadCommFile.readAll());
        if (threadName.trimmed() == QStringLiteral("QSGRenderThread")) {
            eligibleThreads.append(threadId);
        }
    }

    // For now, only support one QSGRenderThread per process
    if (eligibleThreads.length() != 1)
        return;

    bool tidParsed = false;
    int tid = eligibleThreads[0].toInt(&tidParsed, 10);
    
    if (!tidParsed)
        return;

    qInfo() << "FIFO scheduling for tid" << tid;
    struct sched_param param = { .sched_priority = 90 };
    sched_setscheduler(tid, SCHED_FIFO | SCHED_RESET_ON_FORK, &param);
}