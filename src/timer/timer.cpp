#include "timer.h"

#include <esp_timer.h>

esp_timer_handle_t timer;
const esp_timer_create_args_t timer_args = {
    .callback = &timer_interrupt,
    .name = "servo_timer",
};

// Set the timer period in microseconds (e.g., 1 second = 1000000 microseconds)
int64_t timer_period = 1500000; // 15 miliseconds

void startTimer()
{
  // Timer setup
  ESP_ERROR_CHECK(esp_timer_create(&timer_args, &timer));
  ESP_ERROR_CHECK(esp_timer_start_periodic(timer, timer_period));
}

void stopTimer()
{
    // TODO
}
