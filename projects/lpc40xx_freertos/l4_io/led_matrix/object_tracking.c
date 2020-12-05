#include "object_tracking.h"
#include "mp3.h"
#include "shapes.h"
time_t t;
struct object_details onscreen_objects_struct[number_of_objects];

void initialize_object_details() {
  int random;
  friend_score = 0;
  enemy_score = 0;
  // void_function_t draw_enemy_pointer = &draw_enemy;
  for (int i = 0; i < number_of_objects; i++) {
    random = rand() % 63;
    onscreen_objects_struct[i].row = random;
    random = rand() % 63;
    onscreen_objects_struct[i].column = random;
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
      switch (onscreen_objects_struct[i].obj_nature) {

      case FRIEND_OBJECT:
        draw_friend(onscreen_objects_struct[i].row, onscreen_objects_struct[i].column);
        break;

      case ENEMY_OBJECT:
        draw_enemy(onscreen_objects_struct[i].row, onscreen_objects_struct[i].column);
        break;

      default:
        break;
      }
    }
  }
}

void detect_click(uint8_t p, uint8_t q, uint8_t hit) {

  if (hit) {
    uint8_t x = p, y = q;
    p = 0;
    q = 0;

    uint64_t temp;

    temp = frame_buffer[x][ENEMY_PLANE];
    temp = temp >> (63 - y);
    temp = temp << 63;
    led_matrix__set_pixel(x, 63 - y, RED);

    if (temp != 0) {
      for (uint8_t i = 0; i < number_of_objects; i++) {

        if (((onscreen_objects_struct[i].row) <= x) && ((onscreen_objects_struct[i].row) + 7 >= x) &&
            ((onscreen_objects_struct[i].column) <= y) && ((onscreen_objects_struct[i].column) + 7 >= y)) {
          onscreen_objects_struct[i].status = false;
          enemy_score++;
          mp3__send_command(C_PLAY_FOLD_FILE, 0x0301);
          printf("Friendly kill %d Enemy Kill %d\n", friend_score, enemy_score);
          // print_score(friend_score, 0, 0);
        }
      }
    }

    temp = frame_buffer[p][FRIEND_PLANE];
    temp = temp >> (63 - q);
    temp = temp << 63;
    led_matrix__set_pixel(p, 63 - q, YELLOW);

    if (temp != 0) {

      for (uint8_t i = 0; i < number_of_objects; i++) {

        if (((onscreen_objects_struct[i].row) <= p) && ((onscreen_objects_struct[i].row) + 7 >= p) &&
            ((onscreen_objects_struct[i].column) <= q) && ((onscreen_objects_struct[i].column) + 7 >= q)) {
          onscreen_objects_struct[i].status = false;
          friend_score++;

          // mp3__send_command(C_PLAY_W_VOL, 0x1e01);
          printf("Friendly kill %d Enemy Kill %d\n", friend_score, enemy_score);
          // print_score(enemy_score, 0, 32);
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
      // fprintf(stderr, "%d %d\n", x, y);

      for (uint8_t i = 0; i < number_of_objects; i++) {

        if (((onscreen_objects_struct[i].row) <= x) && ((onscreen_objects_struct[i].row) + 7 >= x) &&
            ((onscreen_objects_struct[i].column) <= y) && ((onscreen_objects_struct[i].column) + 7 >= y) &&
            ((onscreen_objects_struct[i].obj_nature) == FRIEND_OBJECT)) {
          onscreen_objects_struct[i].status = false;
          uint32_t temp1 = (uint32_t)(temp & (0xFFFFFFFF));
          temp = temp >> 32;
          uint32_t temp2 = (uint32_t)(temp & (0xFFFFFFFF));
        }
      }
    }
  }
}

uint8_t set_bit_position(uint64_t temp) {
  uint64_t buffer;
  uint8_t column = 0;
  for (uint8_t i = 0; i < 64; i++) {
    buffer = (temp & (1 << i));
    if (buffer) {
      // printf("%d column", (63 - i));
      column = 63 - i;
    }
  }
  return (column);
}