#include "movement.h"

#include <Preferences.h>

#define MOVEMENT_SPACENAME "movement"
#define MOVEMENT_PREF_X_AXIS "x_vals"
#define MOVEMENT_PREF_Y_AXIS "y_vals"
#define MOVEMENT_PREF_LENGTH "mv_len"


void save_movement_algorithm(int* x_axis_points, int* y_axis_points, unsigned int alg_size)
{
    Preferences movement = Preferences();

    movement.begin(MOVEMENT_SPACENAME, false);

    movement.clear();

    movement.putBytes(MOVEMENT_PREF_X_AXIS, x_axis_points, alg_size * sizeof(int));
    movement.putBytes(MOVEMENT_PREF_Y_AXIS, y_axis_points, alg_size * sizeof(int));
    movement.putUInt(MOVEMENT_PREF_LENGTH, alg_size);

    movement.end();
}

void clear_movement_algorithm()
{
    Preferences movement = Preferences();

    movement.begin(MOVEMENT_SPACENAME, false);
    movement.clear();
    movement.end();
}

int read_movement_algorithm(int** x_axis_points_dest, int** y_axis_points_dest)
{
    Preferences movement = Preferences();

    movement.begin(MOVEMENT_SPACENAME, true);

    unsigned int alg_size = movement.getUInt(MOVEMENT_PREF_LENGTH, 0);
    if (alg_size == 0) return alg_size;
    *x_axis_points_dest = (int*) malloc(alg_size * sizeof(int));
    *y_axis_points_dest = (int*) malloc(alg_size * sizeof(int));

    movement.getBytes(MOVEMENT_PREF_X_AXIS, *x_axis_points_dest, alg_size * sizeof(int));
    movement.getBytes(MOVEMENT_PREF_Y_AXIS, *y_axis_points_dest, alg_size * sizeof(int));

    movement.end();

    return alg_size;
}
