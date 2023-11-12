#include <WiFi.h>

#define GET_CREDENTIALS "GET /credentials"
#define PARAM_DIVIDER "&"
#define SSID "ssid="
#define PSWD "pswd="

void wifi_connection_attempt();
void check_wifi_status(bool &ap_running);
void setup_access_point(bool &ap_running);
void turn_off_access_point(bool &ap_running);

bool client_request_listener(bool &ap_running);

void startCameraServer();
