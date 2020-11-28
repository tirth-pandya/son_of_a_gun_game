#include "joystick.h"
#include "zigbee.h"

void joystick_comm__send(void) {
  static joystick__values_s data;
  static uint8_t count = 3;
  if (count == 3) {
    data = joystick__get_value();
    count = 0;
  } else {
    count++;
  }

  // printf("I %d %d\n", data.x, data.y);
  uint8_t message[Max_message_elemets] = {data.x, data.y, data.sw};
  zigbee__data_transfer(message, Max_message_elemets);
}