#include "shapes.h"

const uint8_t enemy_1[8] = {0x08, 0x08, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00},
              enemy_2[8] = {0x00, 0x00, 0x1c, 0x22, 0x00, 0x22, 0x1c, 0x00},
              enemy_3[8] = {0x00, 0x00, 0x00, 0x14, 0x08, 0x08, 0x00, 0x00},
              enemy_fill[8] = {0x08, 0x08, 0x1c, 0x3e, 0x7f, 0x3e, 0x1c, 0x00},

              friend_fill[8] = {0x7c, 0x7f, 0xff, 0xff, 0xfe, 0x7e, 0x66, 0xe7},
              friend_2[8] = {0x00, 0x0f, 0x09, 0x0f, 0x00, 0x00, 0x00, 0x00},
              friend_w1[8] = {0x7c, 0x40, 0xc0, 0xc0, 0xc2, 0x7e, 0x24, 0x43},
              friend_w2[8] = {0x7c, 0x40, 0xc0, 0xc0, 0xc2, 0x7e, 0x42, 0x34},

              heart[8] = {0x50, 0xf8, 0xf8, 0x70, 0x20, 0x00, 0x00, 0x00},
              cursor[8] = {0x40, 0xa0, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00},
              empty[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void draw_life(int x, int y) { shape_update(x, y, heart, RED, LIFE); }

void draw_enemy(int x, int y) {
  shape_update(x, y, enemy_fill, FILL, ENEMY);
  shape_update(x, y, enemy_1, YELLOW, ENEMY);
  shape_update(x, y, enemy_2, BLUE, ENEMY);
  shape_update(x, y, enemy_3, RED, ENEMY);
}

void draw_friend(int x, int y) {

  shape_update(x, y, friend_fill, FILL, FRIEND);

  static int count_frame = 0, count_frame2 = 0;
  if (count_frame < 10) {
    shape_update(x, y, friend_w2, YELLOW, FRIEND);
    count_frame++;
    count_frame2 = 0;
  }
  if (count_frame == 10 && count_frame2 < 10) {
    shape_update(x, y, friend_w1, YELLOW, FRIEND);
    count_frame2++;
    if (count_frame2 == 9)
      count_frame = 0;
  }
  // shape_update(10, 10, heart, RED, FRIEND);
  shape_update(x, y, friend_2, CYAN, FRIEND);
}

/**
 * This function first cleares the RGB plane where the object is to be placed.
 * This avoids color mixing. Foreground/background object placement is achived here.
 * Objects which are drawn using this function...the one which is called last is printed on top
 * of the objects which were drawn on previous calls.
 * Only clear part where object is to be drawn and not the complete image.
 * Clear image plane on each refresh cycle.
 */
void shape_update(int row, int column, const uint8_t *shape, led_matrix__color_e shape_color,
                  game_object_type object_type) {
  uint8_t size_column = 8, size_row = 8;
  uint64_t temp_row = 0;

  column = 63 - column - size_column;

  for (uint8_t i = 0; i < size_row; i++) {

    if (((row + i) >= 0) && ((row + i) <= 63)) {
      temp_row = shape[i];

      if (column >= 0)
        temp_row = temp_row << column;
      else
        temp_row = temp_row >> (column * -1);

      temp_row = ~temp_row;

      frame_buffer[row + i][BLUE_PLANE] &= temp_row;
      frame_buffer[row + i][RED_PLANE] &= temp_row;
      frame_buffer[row + i][GREEN_PLANE] &= temp_row;

      temp_row = shape[i];
      if (column >= 0)
        temp_row = temp_row << column;
      else
        temp_row = temp_row >> (column * -1);

      switch (shape_color) {
      case OFF:
        temp_row = 0;
        frame_buffer[row + i][BLUE_PLANE] |= temp_row;
        frame_buffer[row + i][RED_PLANE] |= temp_row;
        frame_buffer[row + i][GREEN_PLANE] |= temp_row;
        break;
      case BLUE:
        frame_buffer[row + i][BLUE_PLANE] |= temp_row;
        break;
      case GREEN:
        frame_buffer[row + i][GREEN_PLANE] |= temp_row;
        break;
      case CYAN:
        frame_buffer[row + i][BLUE_PLANE] |= temp_row;
        frame_buffer[row + i][GREEN_PLANE] |= temp_row;
        break;
      case RED:
        frame_buffer[row + i][RED_PLANE] |= temp_row;
        break;
      case MAGENTA:
        frame_buffer[row + i][BLUE_PLANE] |= temp_row;
        frame_buffer[row + i][RED_PLANE] |= temp_row;
        break;
      case YELLOW:
        frame_buffer[row + i][RED_PLANE] |= temp_row;
        frame_buffer[row + i][GREEN_PLANE] |= temp_row;
        break;
      case WHITE:
        frame_buffer[row + i][BLUE_PLANE] |= temp_row;
        frame_buffer[row + i][RED_PLANE] |= temp_row;
        frame_buffer[row + i][GREEN_PLANE] |= temp_row;
        break;

      case FILL: {
        ;
      } break;

      default:
        break;
      }

      switch (object_type) {
      case FRIEND:
        frame_buffer[row + i][FRIEND_PLANE] |= temp_row;
        break;
      case ENEMY:
        frame_buffer[row + i][ENEMY_PLANE] |= temp_row;
        break;
      case LIFE:
        frame_buffer[row + i][LIFE_PLANE] |= temp_row;
        break;
      default:
        break;
      }
    }
  }
}

void print_score(uint8_t score, uint8_t x, uint8_t y, led_matrix__color_e shape_color) {
  int temp_int = 0;
  char temp_char[3];

  temp_int = score % 10;
  temp_char[2] = '0'; // temp_int + '0';

  temp_int = score / 10;
  temp_char[1] = temp_int + '0';

  print_char(temp_char, x, y, shape_color);
}
