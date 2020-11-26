#pragma once
#include "shapes.h"
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define number_of_objects 3

typedef enum { FRIEND_OBJECT = 0, ENEMY_OBJECT } OBJECT_NATURE;

struct object_details {
  int row, column;
  uint8_t *ptr;
  OBJECT_NATURE obj_nature;
  bool status;
};

void draw_from_structure();
void randomizer_objects();
void initialize_object_details();