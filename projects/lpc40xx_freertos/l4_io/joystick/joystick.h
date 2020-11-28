#pragma once

#include "gpio.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  uint8_t x, y;
  uint8_t sw;
} joystick__values_s;

joystick__values_s joystick__get_value(void);
void joystick__initialize(gpio_s joystick__x_axis, gpio_s joystick__y_axis, gpio_s joystick__switch);
float map(long x, long in_min, long in_max, float out_min, float out_max);