#include "object_tracking.h"
#include "shapes.h"

time_t t;
struct object_details onscreen_objects_struct[number_of_objects];

void initialize_object_details() {
  int random;
  friend_score = 0;
  enemy_score = 0;
  for (int i = 0; i < number_of_objects; i++) {
    random = rand() % 63;
    onscreen_objects_struct[i].row = random;
    random = rand() % 63;
    onscreen_objects_struct[i].column = random;
    onscreen_objects_struct[i].ptr = test;
    onscreen_objects_struct[i].obj_nature = i % 2;
    onscreen_objects_struct[i].status = true;
  }
}

void randomizer_objects() {
  int random;
  for (int i = 0; i < number_of_objects; i++) {
    random = rand() % 3;
    random = random - 1;
    onscreen_objects_struct[i].row += random;
    if ((onscreen_objects_struct[i].row < -8) || (onscreen_objects_struct[i].row > 71))
      onscreen_objects_struct[i].row = rand() % 63;
    random = rand() % 3;
    random = random - 1;
    onscreen_objects_struct[i].column += random;
    if ((onscreen_objects_struct[i].column < -8) || (onscreen_objects_struct[i].column > 71))
      onscreen_objects_struct[i].column = rand() % 63;
    // printf("%d %d %d\n", onscreen_objects_struct[i].row, onscreen_objects_struct[i].column, i);
  }
}

void draw_from_structure() {

  for (uint8_t i = 0; i < number_of_objects; i++) {
    if (onscreen_objects_struct[i].status) {
      shape_update(onscreen_objects_struct[i].row, onscreen_objects_struct[i].column, onscreen_objects_struct[i].ptr,
                   1 + (i % 2), onscreen_objects_struct[i].obj_nature);
    }
  }
}

void detect_click(uint8_t hit) {

  if (hit) {
    uint8_t x = 50, y = 53, p = 10, q = 22;

    uint64_t temp;

    temp = frame_buffer[x][ENEMY_PLANE];
    temp = temp >> 63 - y;
    temp = temp << 63;
    led_matrix__set_pixel(x, 63 - y, RED);

    if (temp != 0) {
      for (uint8_t i = 0; i < number_of_objects; i++) {

        if (((onscreen_objects_struct[i].row) <= x) && ((onscreen_objects_struct[i].row) + 7 >= x) &&
            ((onscreen_objects_struct[i].column) <= y) && ((onscreen_objects_struct[i].column) + 7 >= y)) {
          onscreen_objects_struct[i].status = false;
          enemy_score++;
          printf("Friendly kill %d Enemy Kill %d\n", friend_score, enemy_score);
        }
      }
    }

    temp = frame_buffer[p][FRIEND_PLANE];
    temp = temp >> 63 - q;
    temp = temp << 63;
    led_matrix__set_pixel(p, 63 - q, YELLOW);

    if (temp != 0) {

      for (uint8_t i = 0; i < number_of_objects; i++) {

        if (((onscreen_objects_struct[i].row) <= p) && ((onscreen_objects_struct[i].row) + 7 >= p) &&
            ((onscreen_objects_struct[i].column) <= q) && ((onscreen_objects_struct[i].column) + 7 >= q)) {
          onscreen_objects_struct[i].status = false;
          friend_score++;
          printf("Friendly kill %d Enemy Kill %d\n", friend_score, enemy_score);
        }
      }
    }
  }
}

void collision_detection() {
  uint8_t x, y;
  uint64_t temp;
  for (uint8_t j = 0; j < 64; j++) {
    temp = frame_buffer[j][FRIEND_PLANE] & frame_buffer[j][ENEMY_PLANE];
    if (temp) {
      x = j;
      y = set_bit_position(temp);
      printf("%llu", temp);

      for (uint8_t i = 0; i < number_of_objects; i++) {

        if (((onscreen_objects_struct[i].row) <= x) && ((onscreen_objects_struct[i].row) + 7 >= x) &&
            //((onscreen_objects_struct[i].column) <= y) && ((onscreen_objects_struct[i].column) + 7 >= y) &&
            ((onscreen_objects_struct[i].obj_nature) == FRIEND_OBJECT)) {
          onscreen_objects_struct[i].status = false;
        }
      }
    }
  }
}

uint8_t set_bit_position(uint64_t temp) {
  uint64_t buffer;
  for (uint8_t i = 0; i < 64; i++) {
    buffer = (temp & (1 < i));
    if (buffer == 1) {
      printf("%d", (63 - i));
      return (63 - i);
    }
  }
}