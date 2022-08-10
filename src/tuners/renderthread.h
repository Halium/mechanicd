#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QObject>

class RenderThreadTuner : public QObject
{
    Q_OBJECT

public:
    RenderThreadTuner(QObject* parent = nullptr);

    void tunePid(pid_t pid);
};

#endif