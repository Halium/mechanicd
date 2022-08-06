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
    PowerInterface() = default;
    virtual ~PowerInterface() {}

Q_SIGNALS:
    virtual void screenStateChanged(ScreenState) = 0;
};
Q_DECLARE_INTERFACE(PowerInterface, "PowerInterface")

#endif