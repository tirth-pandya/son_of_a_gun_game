#include "graphics.h"
#include <stdint.h>

const data_size ALL_LEDS_ON = 0xFFFFFFFFFFFFFFFF;
const data_size ALL_LEDS_OFF = 0x00;

void graphics__turn_on_all_leds(led_matrix__color_e color) { led_matrix__fill_data_buffer(ALL_LEDS_ON, color); }

void graphics__turn_off_all_leds(void) { led_matrix__clear_data_buffer(); }

void graphics_print_test_row() { led_matrix__set_pixel(0, 0, BLUE); }
