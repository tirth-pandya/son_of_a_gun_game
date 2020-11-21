#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  int16_t x, y;
  // bool sw;
} joystick__values_s;

joystick__values_s joystick__get_value(void);
void joystick__initialize(gpio_s joystick__x_axis, gpio_s joystick__y_axis, gpio_s joystick__switch);
long map(long x, long in_min, long in_max, long out_min, long out_max);