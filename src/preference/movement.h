#if !defined(CAMERA_PREFERENCE_MOVEMENT)
#define CAMERA_PREFERENCE_MOVEMENT

void save_movement_algorithm(int* x_axis_points, int* y_axis_points, unsigned int alg_size);

void clear_movement_algorithm();

int read_movement_algorithm(int** x_axis_points_dest, int** y_axis_points_dest);

#endif // CAMERA_PREFERENCE_MOVEMENT
