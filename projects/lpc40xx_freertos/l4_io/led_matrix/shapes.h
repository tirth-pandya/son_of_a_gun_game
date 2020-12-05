#pragma once
#include "led_matrix.h"
#include <stdint.h>

typedef enum { FRIEND, ENEMY, LIFE } game_object_type;
extern const uint8_t empty[8], test[8], a1[8], a2[8], a3[8], data4[8], cursor[8];

/**
 * Shape update draws the shape which has been passed as parameters to the function.
 * Parameters are self explanatory.
 * Shapes are created in matrix in .c file.
 * Virtual plane of friend, enemy and life is also filled for object type detection, scoring and gameplay.
 */
void shape_update(int row, int column, const uint8_t *shape, led_matrix__color_e shape_color,
                  game_object_type object_type);

/**
 * Draw enemy shape for given co-ordinates.
 */
void draw_enemy(int x, int y);

/**
 * Draw friend objects as given co-ordinates.
 */
void draw_friend(int x, int y);
