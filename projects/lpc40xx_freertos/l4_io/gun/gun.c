/**
 * @file gun.c
 * 
 * @brief This file is responsible for maintaining APIs interfacing with accelerometer and gun parameters
 *        to be output. 
 */

#include "gun.h"
#include "acceleration.h"

gun_param gun__get_current_param(uint8_t samples_to_avg, uint16_t sensitivity) {
  acceleration__axis_data_s accel_coord = acceleration__get_averaged_data(samples_to_avg, sensitivity);
  // As per current orientation of SJ2 board on Gun, column change i.e. X axis on Led matrix is mapped to
  // Y coordinate of accelerometer. And Row change i.e. Y axis on LED matrix is mapped to Z coordinate on accelerometer.
  gun_param current_status = {(accel_coord.y), (63 - accel_coord.z), 0};
  return current_status;
}

