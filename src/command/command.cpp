#include "command.h"

#include <cstring>

#include "../timer/timer.h"
#include "../algorithm/algorithm.h"
#include "../servo/servo_control.h"

char* direction = SERVO_CMD_STOP;


int cmd_move(char *new_direction)
{
    if (new_direction == NULL) return 1;

    static char* valid_commands[] = {
        SERVO_CMD_DOWN,
        SERVO_CMD_UP,
        SERVO_CMD_LEFT,
        SERVO_CMD_RIGHT,
        SERVO_CMD_STOP,
        SERVO_CMD_FOLLOW_ROUTE
    };

    for (int i = 0; i < sizeof(valid_commands); i++) {
        if (!strcmp(new_direction, valid_commands[i])) {
            direction = valid_commands[i];
            if (!strcmp(new_direction, SERVO_CMD_STOP)) {
                stopTimer();
            } else {
                startTimer();
            }
            return 0;
        }
    }
    return 1;
}

int cmd_refresh_record()
{
    start_algorithm_writing();
    update_path();
    return 0;
}

int cmd_add_point()
{
    add_new_point(get_servo_x_coord(), get_servo_y_coord());
    update_path();
    return 0;
}

char *get_current_direction()
{
    return direction;
}
