#include "schedtune.h"

#include <QDebug>
#include <QFile>
#include <QDir>
#include <QString>
#include <QTextStream>

SchedTuneTuner::SchedTuneTuner(QObject* parent) : QObject(parent)
{
    
}


void SchedTuneTuner::tunePid(pid_t pid)
{
    const QString cgroups = QStringLiteral("/proc/%1/cgroup").arg(QString::number(pid));

    QFile cgroupsFile(cgroups);
    if (!cgroupsFile.exists())
        return;

    if (!cgroupsFile.open(QFile::ReadOnly)) {
        qWarning() << "Failed to open cgroups file for pid" << pid;
        return;
    }

    const QString cgroupAssignments = QString::fromUtf8(cgroupsFile.readAll());
    if (cgroupAssignments.isEmpty()) {
        qWarning() << cgroups << "is empty";
        return;
    }

    const QStringList cgroupControllers = cgroupAssignments.split('\n', QString::SkipEmptyParts);
    if (cgroupControllers.length() == 0) {
        qWarning() << "Failed to read" << cgroups << "as lines";
        return;
    }

    for (const QString& cgroupController : cgroupControllers) {
        const QStringList cgroupParts = cgroupController.split(':', QString::SkipEmptyParts);
        if (cgroupParts.length() != 3)
            continue;

        if (cgroupParts[1] != QStringLiteral("schedtune"))
            continue;


        QDir schedtuneDir("/sys/fs/cgroup/schedtune/mechanicd");

        if (!schedtuneDir.exists()) {
            schedtuneDir.mkdir("/sys/fs/cgroup/schedtune/mechanicd");
        }

        if (!schedtuneDir.exists())
            continue;

        // Main cgroup control file for adjusting frequency boosting
        {
            const QString booster = QStringLiteral("/sys/fs/cgroup/schedtune/mechanicd/%1/schedtune.boost").arg(cgroupParts[2]);
            QFile boosterFile(booster);

            if (!boosterFile.exists())
                continue;

            if (!boosterFile.open(QFile::WriteOnly))
                continue;

            QTextStream textStream(&boosterFile);
            textStream << "20";
        }

        // Optional schedtune value found on the JingPad
        {
            const QString load_pct = QStringLiteral("/sys/fs/cgroup/schedtune/mechanicd/%1/schedtune.init_task_load_pct").arg(cgroupParts[2]);
            QFile loadPctFile(load_pct);

            if (!loadPctFile.exists())
                continue;

            if (!loadPctFile.open(QFile::WriteOnly))
                continue;

            QTextStream textStream(&loadPctFile);
            textStream << "45";
        }

        // Write the pid
        {
            const QString cgroup_procs = QStringLiteral("/sys/fs/cgroup/schedtune/mechanicd/%1/cgroup.procs").arg(cgroupParts[2]);
            QFile cgroupProcsFile(cgroup_procs);

            if (!cgroupProcsFile.exists())
                continue;

            if (!cgroupProcsFile.open(QFile::WriteOnly))
                continue;

            QTextStream textStream(&cgroupProcsFile);
            textStream << QString::number(pid);
        }
    }
}
