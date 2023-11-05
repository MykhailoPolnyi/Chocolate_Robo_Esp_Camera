#if !defined(ESP32_CAMERA_COMMAND)
#define ESP32_CAMERA_COMMAND

#define SERVO_CMD_STOP "stop"
#define SERVO_CMD_UP "up"
#define SERVO_CMD_DOWN "down"
#define SERVO_CMD_LEFT "left"
#define SERVO_CMD_RIGHT "right"
#define SERVO_CMD_FOLLOW_ROUTE "route"

int cmd_move(char* new_direction);

int cmd_refresh_record();

int cmd_add_point();

char* get_current_direction();

#endif // ESP32_CAMERA_COMMAND
