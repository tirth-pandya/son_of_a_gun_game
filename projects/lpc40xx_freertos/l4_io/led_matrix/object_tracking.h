#pragma once
#include "shapes.h"
#include "zigbee.h"
#include <stdbool.h>
#include <stdint.h>
//#include <time.h>

#define number_of_objects 15

// typedef void (*void_function_t)(int x, int y);

typedef enum { FRIEND_OBJECT = 0, ENEMY_OBJECT, LIFE_OBJECT, BLAST_ENEMY } OBJECT_NATURE;

struct object_details {
  int row, column;
  OBJECT_NATURE obj_nature;
  bool status;
};

typedef struct obj_detail {
  OBJECT_NATURE obj_nat;
  bool obj_stat;
} obj_details_s;

/**
 *  Draw from structure accesses the structure elements to get the x,y co-ordinate
 * and status and object_type to draw the shape
 */
void draw_from_structure();

/**
 * Randomizer changes the position of objects which needs to be changed in gameplay
 * This affects the .x and.y element in structure
 */
void randomizer_objects_level_3();

/**
 * Randomizer changes the position of objects which needs to be changed in gameplay
 * This affects the .x and.y element in structure
 * Level 1
 * Row is constant, Column varies by --1
 */
void randomizer_objects_level_1();

/**
 * Randomizer changes the position of objects which needs to be changed in gameplay
 * This affects the .x and.y element in structure
 * Level 2
 * Ro
 */
void randomizer_objects_level_2();

/**
 * Initialize the maximum number of objects with random/pseudo co-ordinate.
 * This is idially executed once as it is an initialize function.
 */
void initialize_object_details();

/**
 * Static objects when game is over
 */
void static_object_at_game_over();

/**
 * Detect the click from the gun.
 * Once the click is detected, use the appropriate friend,enemy plane to determine the
 * location of cursor on the object_type and take approriate action
 */
void detect_click(uint8_t p, uint8_t q, uint8_t hit);

/**
 * Detect intersection of two objects with friend/enemy object type and take appropriate action
 */
void collision_detection();

/**
 * Detect intersection of two objects with friend/life object type and take appropriate action
 */
void collision_detection_for_life();

/**
 * Determine the position of first 1 in uint64_t for determining the column position
 */
uint8_t set_bit_position(uint64_t temp);

void update_friend_location();

/**
 * This function revives the given number of enemies required for the current game play level
 */
void update_required_enemies_status(int number_of_enemies);

/**
 * This funciton revive the life object with probability assigned to each game play level
 */
void object_tracking__revive_life_object(void);

void set_onscreen_object_details(int struct_pos, OBJECT_NATURE nature, bool status_onscreen);

obj_details_s get_onscreen_object_details(int struct_pos);

extern bool friend_got_hurt;