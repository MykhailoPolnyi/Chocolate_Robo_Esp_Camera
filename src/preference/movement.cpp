#include "movement.h"

#include <Preferences.h>

#define MOVEMENT_SPACENAME "wifi"
#define MOVEMENT_PREF_X_AXIS "x_vals"
#define MOVEMENT_PREF_Y_AXIS "y_vals"
#define MOVEMENT_PREF_LENGTH "mv_len"

Preferences movement;

void save_movement_algorithm(int* x_axis_points, int* y_axis_points, unsigned int alg_size)
{
    movement.begin(MOVEMENT_SPACENAME, false);

    movement.clear();

    movement.putBytes(MOVEMENT_PREF_X_AXIS, x_axis_points, alg_size);
    movement.putBytes(MOVEMENT_PREF_Y_AXIS, y_axis_points, alg_size);
    movement.putUInt(MOVEMENT_PREF_LENGTH, alg_size);

    movement.end();
}

int read_movement_algorithm(int** x_axis_points_dest, int** y_axis_points_dest)
{
    movement.begin(MOVEMENT_SPACENAME, true);

    unsigned int alg_size = movement.getUInt(MOVEMENT_PREF_LENGTH, 0);
    if (alg_size == 0) return alg_size;
    *x_axis_points_dest = (int*) malloc(alg_size * sizeof(int));
    *y_axis_points_dest = (int*) malloc(alg_size * sizeof(int));

    (int*) movement.getBytes(MOVEMENT_PREF_X_AXIS, *x_axis_points_dest, alg_size);
    (int*) movement.getBytes(MOVEMENT_PREF_Y_AXIS, *y_axis_points_dest, alg_size);

    movement.end();

    return alg_size;
}
