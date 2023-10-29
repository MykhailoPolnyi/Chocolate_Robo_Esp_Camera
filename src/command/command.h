#if !defined(ESP32_CAMERA_COMMAND)
#define ESP32_CAMERA_COMMAND

#define SERVO_CMD_STOP "stop"
#define SERVO_CMD_UP "up"
#define SERVO_CMD_DOWN "down"
#define SERVO_CMD_LEFT "left"
#define SERVO_CMD_RIGHT "right"

int cmd_move(char* new_direction);

extern char* direction;

#endif // ESP32_CAMERA_COMMAND
