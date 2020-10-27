#pragma once

#include <stdbool.h>
#include <stdint.h>

// Data for x-axis, y-axis, and z-axis
typedef struct {
  int16_t x, y, z;
} acceleration__axis_data_s;

typedef enum {
  Portrait_UP = 0,
  Portrait_DOWN = 2,
  Landscape_RIGHT = 4,
  Landscape_LEFT = 6,
  ORIENT_ERROR = 10
} orientation_e;

/// Enables acceleration sensor data update and returns true if we successfully detected the sensor
bool acceleration__init(void);

/// @returns data of all 3-axis of the sensor
acceleration__axis_data_s acceleration__get_data(void);

orientation_e GetOrientation(void);
