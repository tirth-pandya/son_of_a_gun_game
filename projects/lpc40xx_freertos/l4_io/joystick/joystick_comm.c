#include "joystick.h"
#include "zigbee.h"

static joystick__values_s joystick_prev_data = {0, 0, 0};

void joystick_comm__send(void) {
  static joystick__values_s data;

  data = joystick__get_value();

  if ((joystick_prev_data.x != data.x) || (joystick_prev_data.y != data.y) || (joystick_prev_data.sw != data.sw)) {
    joystick_prev_data.x = data.x;
    joystick_prev_data.y = data.y;
    joystick_prev_data.sw = data.sw;
    uint8_t message[Max_message_elemets] = {data.x, data.y, data.sw};
    zigbee__data_transfer(message, Max_message_elemets);
  }

  // static uint8_t count = 2;
  // if (count == 1) {
  //   data = joystick__get_value();
  //   count = 0;
  // } else {
  //   count++;
  // }

  // printf("I %d %d\n", data.x, data.y);
}