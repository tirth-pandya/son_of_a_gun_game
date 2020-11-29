#pragma once
#include "alphabets.h"
#include "led_matrix.h"

/**
 * Print the characters at desired location.
 * Each character has the bitmap assigned.
 */
void print_char(char *ch, u_int8_t row, u_int8_t column, led_matrix__color_e color);
