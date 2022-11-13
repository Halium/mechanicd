#ifndef DBUSWIFIMODESWITCH_H
#define DBUSWIFIMODESWITCH_H

#include <QObject>

class DBusWifiModeSwitch : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.halium.mechanicd.WifiMode")

public:
    explicit DBusWifiModeSwitch(QObject *parent = nullptr);

public Q_SLOTS:
    int requestWifiModeSwitch(int mode);

private:
    const char* (*get_fw_path)(int);
    int (*change_fw_path)(const char*);

    void* wifi_handle;
};

#endif // DBUSWIFIMODESWITCH_H
