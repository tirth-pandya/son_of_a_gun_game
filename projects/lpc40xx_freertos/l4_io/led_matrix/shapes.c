#include "shapes.h"
#include <stdint.h>

const uint8_t test[8] = {0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xfb, 0xfb},
              data1[8] = {0x00, 0x00, 0x00, 0x00, 0x0f, 0x08, 0x08, 0x38},
              data2[8] =
                  {

                      0x00, 0x00, 0x00, 0x00, 0xe0, 0x20, 0xf8, 0x88},
              data3[8] =
                  {

                      0x28, 0x28, 0x28, 0x3f, 0x0f, 0x09, 0x09, 0x01},
              data4[8] = {0xf8, 0x20, 0x20, 0xe0, 0xe0, 0x20, 0x20, 0x00},

              cursor[8] = {0x02, 0x05, 0x02, 0x00, 0x00, 0x00, 0x00, 0x000};
void shape_update(int row, int column, uint8_t shape[], led_matrix__color_e shape_color) {
  uint8_t size_column = 8, size_row = 8;
  uint64_t temp_row = 0;

  column = 63 - column - size_column;
  if (column < 0)
    column = 0;

  for (uint8_t i = 0; i < size_row; i++) {
    printf("%d %d \n", i, shape[i]);
    temp_row = shape[i];
    temp_row = temp_row << column;
    temp_row = ~temp_row;
    frame_buffer[row + i][BLUE_PLANE] &= temp_row;
    frame_buffer[row + i][RED_PLANE] &= temp_row;
    frame_buffer[row + i][GREEN_PLANE] &= temp_row;

    temp_row = shape[i];
    temp_row = temp_row << column;

    switch (shape_color) {
    case OFF:
      temp_row = 0;
      frame_buffer[row + i][BLUE_PLANE] |= temp_row;
      frame_buffer[row + i][RED_PLANE] |= temp_row;
      frame_buffer[row + i][GREEN_PLANE] |= temp_row;
      break;
    case BLUE:
      frame_buffer[row + i][BLUE_PLANE] |= temp_row;
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
    default:
      break;
    }
  }
}
