#ifndef CAMERA_RA_FILTER
#define CAMERA_RA_FILTER

#include "esp_camera.h"

typedef struct ra_filter
{
    size_t size;  //number of values used for filtering
    size_t index; //current value index
    size_t count; //value count
    int sum;
    int* values; //array to be filled with values
} ra_filter_t;

#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
static int ra_filter_run(ra_filter_t* filter, int value);
#endif

#endif // !CAMERA_RA_FILTER
