#include <WiFi.h>

#if !defined(CAMERA_PREFERENCE_WIFI)
#define CAMERA_PREFERENCE_WIFI

void save_wifi(String ssid, String pswd);

String get_wifi_ssid();
String get_wifi_pswd();

#endif // CAMERA_PREFERENCE_WIFI
