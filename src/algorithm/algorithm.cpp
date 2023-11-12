#include "algorithm.h"

#include <malloc.h>

#include "../preference/movement.h"

#define ERR_ON_(cause) if (cause) return -1

unsigned int destination_points_count;

int* x_axis_array;
int* y_axis_array;

void start_algorithm_writing()
{
    destination_points_count = 0;
    free(x_axis_array);
    free(y_axis_array);
    x_axis_array = NULL;
    y_axis_array = NULL;
    clear_movement_algorithm();
}

int add_new_point(int x, int y)
{

    int* realloced_x = (int*) realloc(x_axis_array, (destination_points_count + 1) * sizeof(int));
    ERR_ON_(realloced_x == NULL);

    int* realloced_y = (int*) realloc(y_axis_array, (destination_points_count + 1) * sizeof(int));
    ERR_ON_(realloced_y == NULL);

    x_axis_array = realloced_x;
    y_axis_array = realloced_y;

    x_axis_array[destination_points_count] = x;
    y_axis_array[destination_points_count] = y;

    destination_points_count++;

    save_movement_algorithm(x_axis_array, y_axis_array, destination_points_count);

    return 0;
}
