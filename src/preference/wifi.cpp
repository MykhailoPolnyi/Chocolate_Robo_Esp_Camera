#include "wifi.h"

#include <Preferences.h>

#define WIFI_SPACENAME "wifi";
#define WIFI_PREF_SSID "ssid";
#define WIFI_PREF_PSWD "pswd";

Preferences wifi;

void save_wifi(char *ssid, char *pswd)
{
    wifi.begin(WIFI_SPACENAME, false);

    wifi.clear();

    wifi.putString(WIFI_PREF_SSID, ssid);
    wifi.putString(WIFI_PREF_PSWD, pswd);

    wifi.end();
}

char *get_wifi_ssid()
{
    wifi.begin(WIFI_SPACENAME, true);
    const char* ssid = wifi.getString(WIFI_PREF_SSID, "").c_str();
    wifi.end(WIFI_SPACENAME);
    return ssid;
}

char *get_wifi_pswd()
{
    wifi.begin(WIFI_SPACENAME, true);
    const char* pswd = wifi.getString(WIFI_PREF_PSWD, "").c_str();
    wifi.end();
    return pswd;
}
