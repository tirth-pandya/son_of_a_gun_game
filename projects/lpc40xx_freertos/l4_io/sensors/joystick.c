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
  joystick__values_s joystick_values;
  LPC_IOCON->P0_25 &= ~(0x98);
  joystick_values.x = adc__get_adc_value(ADC__CHANNEL_2);
  LPC_IOCON->P1_30 &= ~(0x98);
  joystick_values.y = adc__get_adc_value(ADC__CHANNEL_4);
  return joystick_values;
}
