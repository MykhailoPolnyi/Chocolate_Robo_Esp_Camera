#if !defined(ESP32_CAMERA_COMMAND_HANDLER)
#define ESP32_CAMERA_COMMAND_HANDLER

#include "esp_http_server.h"
#include "esp_timer.h"

#include "esp_camera.h"
#include "esp32-hal-ledc.h"

#if defined(ARDUINO_ARCH_ESP32) && defined(CONFIG_ARDUHAL_ESP_LOG)
#include "esp32-hal-log.h"
#endif

void setupCommandHandler(httpd_handle_t command_httpd);

#endif // ESP32_CAMERA_COMMAND_HANDLER
