#ifndef SCHEDTUNE_H
#define SCHEDTUNE_H

#include <QObject>

class SchedTuneTuner : public QObject
{
    Q_OBJECT

public:
    SchedTuneTuner(QObject* parent = nullptr);
};

#endif