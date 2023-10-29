#if !defined(ESP32_CAMERA_COMMAND_HANDLER)
#define ESP32_CAMERA_COMMAND_HANDLER

#include "esp_http_server.h"

void setupCommandHandler(httpd_handle_t command_httpd);

#endif // ESP32_CAMERA_COMMAND_HANDLER
