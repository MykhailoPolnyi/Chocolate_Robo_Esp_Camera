#if !defined(CAMERA_PREFERENCE_WIFI)
#define CAMERA_PREFERENCE_WIFI

void save_wifi(char* ssid, char* pswd);

const char* get_wifi_ssid();
const char* get_wifi_pswd();

#endif // CAMERA_PREFERENCE_WIFI
