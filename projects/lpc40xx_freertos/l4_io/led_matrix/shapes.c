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
              empty[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
              blast_1[8] = {0x00, 0x24, 0x66, 0x18, 0x18, 0x66, 0x24, 0x00},
              blast_2[8] = {0x00, 0x5a, 0x18, 0x66, 0x66, 0x18, 0x5a, 0x00};

const uint8_t S_1[8] = {0x00, 0x00, 0x03, 0x07, 0x0c, 0x0c, 0x0c, 0x07},
              S_2[8] = {0x00, 0x00, 0xfc, 0xfe, 0x06, 0x00, 0x00, 0xf8},
              S_3[8] = {0x03, 0x00, 0x00, 0x0c, 0x0f, 0x07, 0x00, 0x00},
              S_4[8] = {0xfc, 0x06, 0x06, 0x06, 0xfc, 0xf8, 0x00, 0x00},
              O_1[8] = {0x00, 0x00, 0x00, 0x01, 0x03, 0x06, 0x06, 0x06},
              O_2[8] = {0x00, 0x00, 0xf8, 0xfc, 0x06, 0x03, 0x03, 0x03},
              O_3[8] = {0x06, 0x06, 0x06, 0x03, 0x01, 0x00, 0x00, 0x00},
              O_4[8] = {0x03, 0x03, 0x03, 0x06, 0xfc, 0xf8, 0x00, 0x00},
              N_1[8] = {0x00, 0x00, 0x0c, 0x0e, 0x0e, 0x0f, 0x0d, 0x0c},
              N_2[8] = {0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x86, 0xc6},
              N_3[8] = {0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x00, 0x00, 0x00},
              N_4[8] = {0x66, 0x36, 0x1e, 0x0e, 0x06, 0x00, 0x00, 0x00},
              G_1[8] = {0x00, 0x00, 0x07, 0x0f, 0x0c, 0x0c, 0x0c, 0x0c},
              G_2[8] = {0x00, 0x00, 0xfe, 0xfe, 0x00, 0x00, 0x00, 0x7c},
              G_3[8] = {0x0c, 0x0c, 0x0c, 0x0f, 0x07, 0x00, 0x00, 0x00},
              G_4[8] = {0x7e, 0x06, 0x06, 0xfc, 0xf8, 0x00, 0x00, 0x00},
              U_1[8] = {0x00, 0x00, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c},
              U_2[8] = {0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06},
              U_3[8] = {0x0c, 0x0c, 0x0c, 0x07, 0x03, 0x00, 0x00, 0x00},
              U_4[8] = {0x06, 0x06, 0x06, 0xfc, 0xf8, 0x00, 0x00, 0x00},
              sO_1[8] = {0x1c, 0x3e, 0x63, 0xc1, 0xc1, 0xc1, 0xc1, 0x63},
              sO_2[8] = {0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00},
              sO_3[8] = {0x3e, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
              sO_4[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
              sF_1[8] = {0x7f, 0xff, 0xc0, 0xc0, 0xfe, 0xfe, 0xc0, 0xc0},
              sF_2[8] = {0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
              sF_3[8] = {0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
              sF_4[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
              sA_1[8] = {0x1c, 0x3e, 0x63, 0xc1, 0xc1, 0xc1, 0xff, 0xff},
              sA_2[8] = {0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80},
              sA_3[8] = {0xc1, 0xc1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
              sA_4[8] = {0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
              gl1[8] = {0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x08, 0x11},
              gl2[8] = {0x30, 0x48, 0x88, 0x10, 0x20, 0x40, 0x80, 0x80},
              gl3[8] = {0x22, 0x42, 0x43, 0x31, 0x18, 0x0c, 0x06, 0x03},
              gl4[8] = {0x40, 0x80, 000, 0x00, 0x80, 0x40, 0x80, 0x00},
              gr1[8] = {0x0c, 0x12, 0x11, 0x08, 0x04, 0x02, 0x01, 0x01},
              gr2[8] = {0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x88},
              gr3[8] = {0x02, 0x01, 0x00, 000, 0x01, 0x02, 0x01, 0x00},
              gr4[8] = {0x44, 0x42, 0xc2, 0x8c, 0x18, 0x30, 0x60, 0xc0};
// sO_1[8] = {0x1c, 0x3e, 0x41, 0xc1, 0xc1, 0xc1, 0xc1, 0x41},
// sO_2[8] = {0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00},
// sO_3[8] = {0x3e, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
// sO_4[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},

const uint8_t crown_1[8] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
              crown_2[8] = {0x11, 0x92, 0xd6, 0xfe, 0x00, 0xfe, 0x00, 0x00};

const int row_boundary_upper = 20, row_boundary_lower = 50;

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

// Always make status true first and false at the end and change object nature to ENEMY
void draw_blast(int struct_pos, int x, int y) {
  obj_details_s blast_obj_details;
  blast_obj_details = get_onscreen_object_details(struct_pos);

  if (blast_obj_details.obj_nat == BLAST_ENEMY) {
    set_onscreen_object_details(struct_pos, blast_obj_details.obj_nat, true); // Status set to true
    static int count_frame1 = 0, count_frame2 = 0;
    if (count_frame1 < 3) {
      shape_update(x, y, blast_1, RED, NONE);
      // shape_update(x, y, blast_2, YELLOW, NONE);
      count_frame1++;
      count_frame2 = 0;
    }
    if (count_frame1 == 3 && count_frame2 < 3) {
      shape_update(x, y, blast_2, GREEN, NONE);
      count_frame2++;
      if (count_frame2 == 2) {
        count_frame1 = 0;
        set_onscreen_object_details(struct_pos, ENEMY, false);
      }
    }
  }
}

void draw_welcome(int x, int y) {
  shape_update(x, y, S_1, RED, NONE);
  shape_update(x, y + 8, S_2, RED, NONE);
  shape_update(x + 8, y, S_3, YELLOW, NONE);
  shape_update(x + 8, y + 8, S_4, YELLOW, NONE);

  shape_update(x, y + 11, O_1, RED, NONE);
  shape_update(x, y + 19, O_2, RED, NONE);
  shape_update(x + 8, y + 11, O_3, YELLOW, NONE);
  shape_update(x + 8, y + 19, O_4, YELLOW, NONE);

  shape_update(x, y + 24, N_1, RED, NONE);
  shape_update(x, y + 32, N_2, RED, NONE);
  shape_update(x + 8, y + 24, N_3, YELLOW, NONE);
  shape_update(x + 8, y + 32, N_4, YELLOW, NONE);

  shape_update(x + 16, y + 5, sO_1, MAGENTA, NONE);
  shape_update(x + 16, y + 13, sO_2, MAGENTA, NONE);
  shape_update(x + 24, y + 5, sO_3, CYAN, NONE);
  shape_update(x + 24, y + 13, sO_4, CYAN, NONE);

  shape_update(x + 16, y + 15, sF_1, MAGENTA, NONE);
  shape_update(x + 16, y + 23, sF_2, MAGENTA, NONE);
  shape_update(x + 24, y + 15, sF_3, CYAN, NONE);
  shape_update(x + 24, y + 23, sF_4, CYAN, NONE);

  shape_update(x + 16, y + 29, sA_1, MAGENTA, NONE);
  shape_update(x + 16, y + 37, sA_2, MAGENTA, NONE);
  shape_update(x + 24, y + 29, sA_3, CYAN, NONE);
  shape_update(x + 24, y + 37, sA_4, CYAN, NONE);

  shape_update(x + 26, y, G_1, RED, NONE);
  shape_update(x + 26, y + 8, G_2, RED, NONE);
  shape_update(x + 34, y, G_3, YELLOW, NONE);
  shape_update(x + 34, y + 8, G_4, YELLOW, NONE);

  shape_update(x + 26, y + 12, U_1, RED, NONE);
  shape_update(x + 26, y + 20, U_2, RED, NONE);
  shape_update(x + 34, y + 12, U_3, YELLOW, NONE);
  shape_update(x + 34, y + 20, U_4, YELLOW, NONE);

  shape_update(x + 26, y + 24, N_1, RED, NONE);
  shape_update(x + 26, y + 32, N_2, RED, NONE);
  shape_update(x + 34, y + 24, N_3, YELLOW, NONE);
  shape_update(x + 34, y + 32, N_4, YELLOW, NONE);

  shape_update(x + 40, y - 5, gl1, WHITE, NONE);
  shape_update(x + 40, y + 3, gl2, WHITE, NONE);
  shape_update(x + 48, y - 5, gl3, WHITE, NONE);
  shape_update(x + 48, y + 3, gl4, WHITE, NONE);

  draw_crown(x + 45, y + 12);

  shape_update(x + 40, y + 33, gr1, WHITE, NONE);
  shape_update(x + 40, y + 41, gr2, WHITE, NONE);
  shape_update(x + 48, y + 33, gr3, WHITE, NONE);
  shape_update(x + 48, y + 41, gr4, WHITE, NONE);
}

void draw_crown(int x, int y) {
  shape_update(x, y - 8, crown_1, YELLOW, NONE);
  shape_update(x, y, crown_2, YELLOW, NONE);
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
  char temp_char[2];

  temp_int = score % 10;
  temp_char[1] = temp_int + '0';

  temp_int = score / 10;
  temp_char[0] = temp_int + '0';

  print_char(temp_char, x, y, shape_color);
}

void draw_road() {
  uint64_t temp;
  static uint64_t solid_road = -1;
  static uint64_t dashed_road = 0xFF00FF00FF00FF00;
  led_matrix__set_row_data(row_boundary_upper, WHITE, solid_road);
  led_matrix__set_row_data(row_boundary_lower, WHITE, solid_road);

  temp = dashed_road >> 63;
  dashed_road = dashed_road << 1;
  dashed_road = dashed_road | temp;
  led_matrix__set_row_data((row_boundary_lower + row_boundary_upper) / 2, WHITE, dashed_road);
}
