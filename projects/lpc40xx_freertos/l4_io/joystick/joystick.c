#include <stdint.h>

#include "adc.h"
#include "gpio.h"
#include "joystick.h"
#include "lpc40xx.h"
#include "lpc_peripherals.h"

// Public

void joystick__initialize(gpio_s joystick__x_axis, gpio_s joystick__y_axis, gpio_s joystick__switch) {
  adc__initialize();

  gpio__construct_with_function(joystick__x_axis.port_number, joystick__x_axis.pin_number, GPIO__FUNCTION_1);
  gpio__construct_with_function(joystick__y_axis.port_number, joystick__y_axis.pin_number, GPIO__FUNCTION_1);
  gpio__set_as_input(joystick__switch);
}

joystick__values_s joystick__get_value(void) {
  joystick__values_s joystick_values = {0, 0};
  static joystick__values_s new_val = {32, 32};
  int x_raw, y_raw;

  LPC_IOCON->P0_25 &= ~(0x98);
  x_raw = adc__get_adc_value(ADC__CHANNEL_2);
  joystick_values.x = map(x_raw, 0, 4096, 1, -2);
  LPC_IOCON->P1_30 &= ~(0x98);
  y_raw = adc__get_adc_value(ADC__CHANNEL_4);
  joystick_values.y = map(y_raw, 0, 4096, -3, 2);

  new_val.x = joystick_values.x + new_val.x;
  if (new_val.x > 63)
    new_val.x = 63;
  if (new_val.x <= 0)
    new_val.x = 0;

  new_val.y = joystick_values.y + new_val.y;
  if (new_val.y > 63)
    new_val.y = 63;
  if (new_val.y <= 0)
    new_val.y = 0;

  return new_val;
}

float map(long x, long in_min, long in_max, float out_min, float out_max) {
  return (float)((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}
