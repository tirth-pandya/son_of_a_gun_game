#pragma once
#include "alphabets.h"
#include "led_matrix.h"
#include "object_tracking.h"
#include <stdint.h>

typedef enum { FRIEND, ENEMY, LIFE, NONE } game_object_type;
extern const uint8_t empty[8], test[8], enemy_1[8], enemy_2[8], enemy_3[8], friend_fill[8], friend_2[8], friend_3[8],
    friend_w1[8], friend_w2[8], data4[8], cursor[8];

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

/**
 * Print score
 */
void print_score(uint8_t score, uint8_t x, uint8_t y, led_matrix__color_e shape_color);
