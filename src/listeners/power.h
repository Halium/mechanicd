#ifndef POWERLISTENER_H
#define POWERLISTENER_H

#include "power/interface.h"

class PowerListener : public QObject, public PowerInterface
{
    Q_OBJECT
    Q_INTERFACES(PowerInterface)

public:
    PowerListener(QObject* parent = nullptr);
};

#endif