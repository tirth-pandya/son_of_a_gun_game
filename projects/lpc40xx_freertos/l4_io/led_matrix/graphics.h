#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_
#include "led_matrix.h"

void graphics__turn_on_all_leds(led_matrix__color_e color);
void graphics__turn_off_all_leds(void);

void led_matrix__drawBall(int row, int column, led_matrix__color_e ball_color);

void graphics_print_ashok_chakra();

void print_char(char *ch, u_int8_t row, u_int8_t column, led_matrix__color_e color);
#endif