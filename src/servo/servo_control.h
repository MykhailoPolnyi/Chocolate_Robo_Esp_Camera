#if !defined(SERVO_CONTROL)
#define SERVO_CONTROL

#define MAX_X_VALUE 180
#define MAX_Y_VALUE 146

char* update_direction();

int calc_next_x();

int calc_next_y();

int get_servo_x_coord();

int get_servo_y_coord();


#endif // SERVO_CONTROL
