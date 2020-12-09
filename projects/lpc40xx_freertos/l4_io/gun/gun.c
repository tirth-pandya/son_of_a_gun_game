/**
 * @file gun.c
 *
 * @brief This file is responsible for maintaining APIs interfacing with accelerometer and gun parameters
 *        to be output.
 */

#include "gun.h"
#include "acceleration.h"
#include "gpio.h"
#include "gun_comm.h"
#include "lpc40xx.h"
#include "lpc_peripherals.h"
#include "stdio.h"

static const uint8_t gun_button_gpio_port = 2;
static const uint8_t gun_button_gpio_pin = 6;

static void gun_shot_interrupt(void) {
  fprintf(stderr, "hi from gun button isr\n");
  gun_com__set_gunShotValue();
  LPC_GPIOINT->IO2IntClr |= (1 << gun_button_gpio_pin);
}

void gun__init(void) {
  gpio_s gun_shot_gpio = gpio__construct_as_input(gun_button_gpio_port, gun_button_gpio_pin);
  gpio__enable_pull_down_resistors(gun_shot_gpio);
  lpc_peripheral__enable_interrupt(LPC_PERIPHERAL__GPIO, gun_shot_interrupt, "Gun shot received");
  LPC_GPIOINT->IO2IntEnR = (1 << gun_button_gpio_pin);
}

gun_param gun__get_current_param(uint8_t samples_to_avg, uint16_t sensitivity) {
  acceleration__axis_data_s accel_coord = acceleration__get_averaged_data(samples_to_avg, sensitivity);
  // As per current orientation of SJ2 board on Gun, column change i.e. X axis on Led matrix is mapped to
  // Y coordinate of accelerometer. And Row change i.e. Y axis on LED matrix is mapped to Z coordinate on accelerometer.
  gun_param current_status = {(accel_coord.y), (63 - accel_coord.z), 0};
  return current_status;
}
