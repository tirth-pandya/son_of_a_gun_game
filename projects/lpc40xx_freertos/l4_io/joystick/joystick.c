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
  static joystick__values_s prev_val = {0.0};
  static joystick__values_s new_val = {0, 0};

  prev_val = new_val;
  LPC_IOCON->P0_25 &= ~(0x98);
  joystick_values.x = adc__get_adc_value(ADC__CHANNEL_2);
  joystick_values.x = map(joystick_values.x, 0, 4096, -64, 0);
  LPC_IOCON->P1_30 &= ~(0x98);
  joystick_values.y = adc__get_adc_value(ADC__CHANNEL_4);
  joystick_values.y = map(joystick_values.y, 0, 4096, 0, 64);
  if ((joystick_values.x - prev_val.x > 0) || (joystick_values.y - prev_val.y > 0)) {
    prev_val = new_val;
    return new_val = joystick_values;
  } else
    return prev_val;
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
