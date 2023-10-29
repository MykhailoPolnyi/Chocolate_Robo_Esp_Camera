#include "command.h"

#include <cstring>

#include "timer/timer.h"

char* direction = NULL;

int cmd_move(char *new_direction)
{
    if (new_direction == NULL) return 1;

    static char* valid_commands[5] = {
        SERVO_CMD_DOWN,
        SERVO_CMD_UP,
        SERVO_CMD_LEFT,
        SERVO_CMD_RIGHT,
        SERVO_CMD_STOP
    };

    for (int i = 0; i < sizeof(valid_commands); i++) {
        if (!strcmp(new_direction, valid_commands[i])) {
            direction = new_direction;
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
