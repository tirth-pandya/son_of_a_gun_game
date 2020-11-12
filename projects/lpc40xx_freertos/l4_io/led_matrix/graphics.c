#include "graphics.h"

const data_size ALL_LEDS_ON = 0xFFFFFFFFFFFFFFFF;
const data_size ALL_LEDS_OFF = 0x00;

void graphics__turn_on_all_leds(led_matrix__color_e color) { led_matrix__fill_data_buffer(ALL_LEDS_ON, color); }

void graphics__turn_off_all_leds(void) { led_matrix__clear_data_buffer(); }

void led_matrix__drawBall(int row, int column, led_matrix__color_e ball_color) {
  // led_matrix__color_e ball_color = RED;
  led_matrix__set_pixel(row, column, ball_color);
  led_matrix__set_pixel(row, column - 1, ball_color);
  led_matrix__set_pixel(row, column + 1, ball_color);
  led_matrix__set_pixel(row, column - 2, ball_color);
  led_matrix__set_pixel(row, column + 2, ball_color);

  led_matrix__set_pixel(row - 1, column, ball_color);
  led_matrix__set_pixel(row - 1, column - 1, ball_color);
  led_matrix__set_pixel(row - 1, column + 1, ball_color);
  led_matrix__set_pixel(row - 1, column - 2, ball_color);
  led_matrix__set_pixel(row - 1, column + 2, ball_color);

  led_matrix__set_pixel(row + 1, column, ball_color);
  led_matrix__set_pixel(row + 1, column - 1, ball_color);
  led_matrix__set_pixel(row + 1, column + 1, ball_color);
  led_matrix__set_pixel(row + 1, column - 2, ball_color);
  led_matrix__set_pixel(row + 1, column + 2, ball_color);

  led_matrix__set_pixel(row - 2, column, ball_color);
  led_matrix__set_pixel(row - 2, column - 1, ball_color);
  led_matrix__set_pixel(row - 2, column + 1, ball_color);

  led_matrix__set_pixel(row + 2, column, ball_color);
  led_matrix__set_pixel(row + 2, column - 1, ball_color);
  led_matrix__set_pixel(row + 2, column + 1, ball_color);
}

void graphics_print_test_row() { led_matrix__set_pixel(0, 0, BLUE); }