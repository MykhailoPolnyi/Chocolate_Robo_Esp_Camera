#if !defined(CAMERA_PREFERENCE_WIFI)
#define CAMERA_PREFERENCE_WIFI

void save_wifi(char* ssid, char* pswd);

char* get_wifi_ssid();
char* get_wifi_pswd();

#endif // CAMERA_PREFERENCE_WIFI
