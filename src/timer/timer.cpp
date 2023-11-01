#include "timer.h"

esp_timer_handle_t timer;

// Set the timer period in microseconds (e.g., 1 second = 1000000 microseconds)
int64_t timer_period = 20000; // 0.2 second
bool timer_is_running = false;

void setupTimer(const esp_timer_create_args_t timer_args)
{
  ESP_ERROR_CHECK(esp_timer_create(&timer_args, &timer));
}

void startTimer()
{
  if(!timer_is_running) {
    ESP_ERROR_CHECK(esp_timer_start_periodic(timer, timer_period));
    timer_is_running = true;
  }
}

void stopTimer()
{
  if(timer_is_running){
    ESP_ERROR_CHECK(esp_timer_stop(timer));
    timer_is_running = false;
  }
}
