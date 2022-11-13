#include "wifimodeswitch.h"

#include <dlfcn.h>
#include <stddef.h>

#include <hybris/common/dlfcn.h>
#include <hardware_legacy/wifi.h>

DBusWifiModeSwitch::DBusWifiModeSwitch(QObject *parent)
    : QObject(parent), wifi_handle(nullptr), get_fw_path(nullptr), change_fw_path(nullptr)
{
    // WIFI management has moved over to libwifi-hal.
    // Only fall back to libhardware_legacy as a last resort.
    wifi_handle = hybris_dlopen("libwifi-hal.so", RTLD_LAZY);
    if (!wifi_handle)
        wifi_handle = hybris_dlopen("libhardware_legacy.so", RTLD_LAZY);

    if (!wifi_handle)
        return;

    *(void **) (&get_fw_path) = hybris_dlsym(wifi_handle, "wifi_get_fw_path");
    *(void **) (&change_fw_path) = hybris_dlsym(wifi_handle, "wifi_change_fw_path");
}

int DBusWifiModeSwitch::requestWifiModeSwitch(int mode)
{
    if (!(get_fw_path && change_fw_path))
        return -1;

    const char* path = get_fw_path(mode);
    return change_fw_path(path);
}
