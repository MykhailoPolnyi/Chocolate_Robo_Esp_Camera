#include "servo_control.h"
#include "../command/command.h"

#include <cstring>

#define ON_DIRECTION(command, check, update) if (!strcmp(direction, command) && (check)) {update;}

// Servo control variables
int x_current = 0;
int y_current = 0;
int x_step = 1;
int y_step = 1;
int x_dest = 0;
int y_dest = 0;

char* update_direction()
{
    char* direction = get_current_direction();

    if (direction == NULL)
    {
        x_dest = x_current;
        y_dest = y_current;
    }
    
    ON_DIRECTION(SERVO_CMD_DOWN, y_dest != MAX_Y_VALUE, y_dest = MAX_Y_VALUE)
    ON_DIRECTION(SERVO_CMD_UP, y_dest != 0, y_dest = 0)
    ON_DIRECTION(SERVO_CMD_RIGHT, x_dest != 0, x_dest = 0)
    ON_DIRECTION(SERVO_CMD_LEFT, x_dest != MAX_X_VALUE, x_dest = MAX_X_VALUE)

    return direction;
}

int calc_next_x()
{
    if (x_current == x_dest) { return y_current; }
    x_current = (x_current < x_dest) ? (x_current + x_step) : (x_current - x_step);
    return y_current;
    
}

int calc_next_y()
{
    if (y_current == y_dest) { return y_current; }
    y_current = (y_current < y_dest) ? (y_current + y_step) : (y_current - y_step);
    return y_current;
}

int get_servo_x_coord() { return x_current; }

int get_servo_y_coord() { return y_current; }
