/**
 * @file gun.h
 * 
 * @brief This header file is mainting declarations of APIs and variables used for gun interfacing functions
 */

#include "acceleration.h"

typedef struct gun_param {
  int8_t x_coord;
  int8_t y_coord;
  uint8_t button;
} gun_param;

/**
 * This function gets accelerometer parameters as per input sampling and sensitivity parameters and returns
 * relevent parameters required for Gun.
 * @param samples_to_avg Number of samples to be considered for averaging before considering parameter for Gun
 * @param sensitivity The number of raw values change of accelerometer per pixel change
 * @return gun_param The relevent gun parameter to move gun pointer
 */
gun_param gun__get_current_param(uint8_t samples_to_avg, uint16_t sensitivity);
