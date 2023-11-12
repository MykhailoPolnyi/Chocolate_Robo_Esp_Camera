#include "wifi_pref.h"

#include <Preferences.h>
#include <WiFi.h>

#define WIFI_SPACENAME "wifi"
#define WIFI_PREF_SSID "ssid"
#define WIFI_PREF_PSWD "pswd"

Preferences wifi;

void save_wifi(String ssid, String pswd)
{
    wifi.begin(WIFI_SPACENAME, false);

    wifi.clear();

    wifi.putString(WIFI_PREF_SSID, ssid);
    wifi.putString(WIFI_PREF_PSWD, pswd);

    wifi.end();
}

String get_wifi_ssid()
{
    wifi.begin(WIFI_SPACENAME, true);
    String ssid = wifi.getString(WIFI_PREF_SSID, "");
    wifi.end();
    return ssid;
}

String get_wifi_pswd()
{
    wifi.begin(WIFI_SPACENAME, true);
    String pswd = wifi.getString(WIFI_PREF_PSWD, "");
    wifi.end();
    return pswd;
}
