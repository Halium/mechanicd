#ifndef POWERINTERFACE_H
#define POWERINTERFACE_H

#include <QObject>

enum ScreenState {
    SCREENSTATE_UNKNOWN = 0,
    SCREENSTATE_ON,
    SCREENSTATE_OFF,
};
Q_DECLARE_METATYPE(ScreenState);

class PowerInterface
{
public:
    virtual ~PowerInterface() {}

Q_SIGNALS:
    void screenStateChanged(ScreenState);
};
Q_DECLARE_INTERFACE(PowerInterface, "PowerInterface")

#endif