#if !defined(ESP32_SERVO_TIMER)
#define ESP32_SERVO_TIMER

#include <esp_timer.h>

void setupTimer(const esp_timer_create_args_t timer_args);

void startTimer();

void stopTimer();

#endif // ESP32_SERVO_TIMER
