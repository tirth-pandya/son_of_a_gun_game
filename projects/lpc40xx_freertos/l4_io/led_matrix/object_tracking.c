#include "object_tracking.h"
#include "shapes.h"

time_t t;
struct object_details onscreen_objects_struct[number_of_objects];

void initialize_object_details() {
  int random;
  for (int i = 0; i < number_of_objects; i++) {
    random = rand() % 63;
    onscreen_objects_struct[i].row = random;
    random = rand() % 63;
    onscreen_objects_struct[i].column = random;
    onscreen_objects_struct[i].ptr = test;
    onscreen_objects_struct[i].obj_nature = FRIEND_OBJECT;
    onscreen_objects_struct[i].status = true;
  }
}

void randomizer_objects() {
  int random;
  for (int i = 0; i < number_of_objects; i++) {
    random = rand() % 4;
    random = random - 2;
    onscreen_objects_struct[i].row += random;
    random = rand() % 4;
    random = random - 2;
    onscreen_objects_struct[i].column += random;
  }
}

void draw_from_structure() {

  for (uint8_t i = 0; i < number_of_objects; i++) {
    if (onscreen_objects_struct[i].status) {
      shape_update(onscreen_objects_struct[i].row, onscreen_objects_struct[i].column, onscreen_objects_struct[i].ptr,
                   1);
    }
  }
}
