#include "servo_control.h"
#include "../command/command.h"
#include "../preference/movement.h"
#include "malloc.h"

#include <cstring>

#define ON_DIRECTION(command, check, update) if (!strcmp(direction, command) && check) {update}

// Servo control variables
int x_current = 0;
int y_current = 0;
int x_step = 1;
int y_step = 1;
int x_dest = 0;
int y_dest = 0;
int* x_path_arr = (int *)malloc(3 * sizeof(int));
int* y_path_arr = (int *)malloc(3*sizeof(int));
int path_size = 3;

void update_path()
{
    // path_size = read_movement_algorithm(&x_path_arr, &y_path_arr);
    x_path_arr[0] = y_path_arr[0] = 20;
    x_path_arr[1] = y_path_arr[1] = 60;
    x_path_arr[2] = y_path_arr[2] = 40;
}

char* update_direction()
{
    char* direction = get_current_direction();

    if (direction == NULL)
    {
        x_dest = x_current;
        y_dest = y_current;
    }
    
    ON_DIRECTION(SERVO_CMD_DOWN, (y_dest != MAX_Y_VALUE || x_dest != x_current), y_dest = MAX_Y_VALUE; x_dest = x_current;)
    ON_DIRECTION(SERVO_CMD_UP, (y_dest != 0 || x_dest != x_current), y_dest = 0; x_dest = x_current;)
    ON_DIRECTION(SERVO_CMD_RIGHT, (x_dest != 0 || y_dest != y_current), x_dest = 0; y_dest = y_current;)
    ON_DIRECTION(SERVO_CMD_LEFT, (x_dest != MAX_X_VALUE || y_dest != y_current), x_dest = MAX_X_VALUE; y_dest = y_current;)

    static int current_path_point = 0;
    if (!strcmp(direction, SERVO_CMD_FOLLOW_ROUTE)) 
    {
        if (x_path_arr == NULL || y_path_arr == NULL)
        {
            update_path();
            current_path_point = 0;
        }
        
        if (current_path_point >= path_size || x_path_arr == NULL || y_path_arr == NULL)
        {
            y_dest = y_current;
            x_dest = x_current;
            return direction;
        }

        if (x_current == x_dest && y_current == y_dest)
        {
            current_path_point = (current_path_point == path_size-1) ? 0 : current_path_point++;
        }

        if (x_dest != x_path_arr[current_path_point] && y_dest != y_path_arr[current_path_point]) 
        {
            x_dest = x_path_arr[current_path_point];
            y_dest = y_path_arr[current_path_point];
        }

    }
    return direction;
}

int calc_next_x()
{
    if (x_current == x_dest) { return x_current; }
    x_current = (x_current < x_dest) ? (x_current + x_step) : (x_current - x_step);
    return x_current;
    
}

int calc_next_y()
{
    if (y_current == y_dest) { return y_current; }
    y_current = (y_current < y_dest) ? (y_current + y_step) : (y_current - y_step);
    return y_current;
}

int get_servo_x_coord() { return x_current; }

int get_servo_y_coord() { return y_current; }
