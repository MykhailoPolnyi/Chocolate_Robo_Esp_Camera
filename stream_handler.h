#ifndef CAMERA_STREAM_HANDLER
#define CAMERA_STREAM_HANDLER
#include "esp_http_server.h"
#include "esp_timer.h"

#include "esp_camera.h"
#include "esp32-hal-ledc.h"
#include "ra_filter.h"

#if defined(ARDUINO_ARCH_ESP32) && defined(CONFIG_ARDUHAL_ESP_LOG)
#include "esp32-hal-log.h"
#endif

void setupStreamHandler(httpd_handle_t stream_httpd);

#endif // !CAMERA_STREAM_HANDLER
