#include "acceleration.h"
#include "stdio.h"

#include "i2c.h"

static const i2c_e acceleration__sensor_bus = I2C__2;
static const uint8_t acceleration__address = 0x38;

typedef enum {
  acceleration__memory_axis_base = 0x01,
  acceleration__memory_control = 0x2A,
  acceleration__memory_who_am_i = 0x0D,
} acceleration__memory_e;

bool acceleration__init(void) {
  const uint8_t active_mode_with_100Hz = (1 << 0) | (3 << 3);
  i2c__write_single(acceleration__sensor_bus, acceleration__address, acceleration__memory_control,
                    active_mode_with_100Hz);

  const uint8_t who_am_i_expected_value = 0x2A;
  const uint8_t who_am_i_actual_value =
      i2c__read_single(acceleration__sensor_bus, acceleration__address, acceleration__memory_who_am_i);

  return (who_am_i_expected_value == who_am_i_actual_value);
}

acceleration__axis_data_s acceleration__get_data(void) {
  acceleration__axis_data_s axis_data = {0};

  uint8_t axis_raw_data[6] = {0};
  i2c__read_slave_data(acceleration__sensor_bus, acceleration__address, acceleration__memory_axis_base, axis_raw_data,
                       sizeof(axis_raw_data));

  // First byte is MSB then LSB
  axis_data.x = ((uint16_t)axis_raw_data[0] << 8) | axis_raw_data[1];
  axis_data.y = ((uint16_t)axis_raw_data[2] << 8) | axis_raw_data[3];
  axis_data.z = ((uint16_t)axis_raw_data[4] << 8) | axis_raw_data[5];

  // Data is at upper 12-bits, so shift the result down by 4
  axis_data.x /= 16;
  axis_data.y /= 16;
  axis_data.z /= 16;

  return axis_data;
}

acceleration__axis_data_s acceleration__get_averaged_data(uint8_t no_of_samples, uint16_t sensitivity) {
  acceleration__axis_data_s axis_values = {0};

  int32_t x = 0, y = 0, z = 0;
  for (int i = 0; i < no_of_samples; i++) {
    axis_values = acceleration__get_data();
    x += axis_values.x;
    y += axis_values.y;
    z += axis_values.z;
  }
  axis_values.x = x / no_of_samples;
  axis_values.y = y / no_of_samples;
  axis_values.z = z / no_of_samples;

  // axis_values.x = 32 + ((axis_values.x * 32) / sensitivity);
  // if (axis_values.x <= 0)
  //   axis_values.x = 0;
  // if (axis_values.x >= 63)
  //   axis_values.x = 63;
  // axis_values.y = 32 + ((axis_values.y * 32) / sensitivity);
  // if (axis_values.y <= 0)
  //   axis_values.y = 0;
  // if (axis_values.y >= 63)
  //   axis_values.y = 63;

  // axis_values.y = 32 + ((axis_values.y * 32) / sensitivity);
  // if (axis_values.y <= 0)
  //   axis_values.y = 0;
  // if (axis_values.y >= 63)
  //   axis_values.y = 63;
  // axis_values.z = 32 + ((axis_values.z * 32) / sensitivity);
  // if (axis_values.z <= 0)
  //   axis_values.z = 0;
  // if (axis_values.z >= 63)
  //   axis_values.z = 63;

  axis_values.x = 32 + ((axis_values.x * 32) / sensitivity);
  if (axis_values.x <= 0)
    axis_values.x = 0;
  if (axis_values.x >= 63)
    axis_values.x = 63;
  axis_values.z = 32 + ((axis_values.z * 32) / sensitivity);
  if (axis_values.z <= 0)
    axis_values.z = 0;
  if (axis_values.z >= 63)
    axis_values.z = 63;
  axis_values.y = 32 + ((axis_values.y * 32) / sensitivity);
  if (axis_values.y <= 0)
    axis_values.y = 0;
  if (axis_values.y >= 63)
    axis_values.y = 63;

  fprintf(stderr, "accl %d  %d  %d", axis_values.x, axis_values.y, axis_values.z);

  return axis_values;
}