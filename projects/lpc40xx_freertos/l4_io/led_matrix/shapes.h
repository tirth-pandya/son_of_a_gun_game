#pragma once
#include "led_matrix.h"

extern const uint8_t empty[8], test[8], data1[8], data2[8], data3[8], data4[8], cursor[8];

void shape_update(int row, int column, const uint8_t *shape, led_matrix__color_e shape_color);
