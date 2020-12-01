#pragma once
#include "shapes.h"
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define number_of_objects 16
uint8_t friend_score, enemy_score;

// typedef void (*void_function_t)(int x, int y);

typedef enum { FRIEND_OBJECT = 0, ENEMY_OBJECT } OBJECT_NATURE;

struct object_details {
  int row, column;
  OBJECT_NATURE obj_nature;
  bool status;
};

/**
 *  Draw from structure accesses the structure elements to get the x,y co-ordinate
 * and status and object_type to draw the shape
 */
void draw_from_structure();

/**
 * Randomizer changes the position of objects which needs to be changed in gameplay
 * This affects the .x and.y element in structure
 */
void randomizer_objects();

/**
 * Initialize the maximum number of objects with random/pseudo co-ordinate.
 * This is idially executed once as it is an initialize function.
 */
void initialize_object_details();

/**
 * Detect the click from the gun.
 * Once the click is detected, use the appropriate friend,enemy plane to determine the
 * location of cursor on the object_type and take approriate action
 */
void detect_click(uint8_t hit);

/**
 * Detect interseaction of two objects with different object type and take appropriate action
 */
void collision_detection();

/**
 * Determine the position of first 1 in uint64_t for determining the column position
 */
uint8_t set_bit_position(uint64_t temp);