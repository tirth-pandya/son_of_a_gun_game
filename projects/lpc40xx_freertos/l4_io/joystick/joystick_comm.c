#include "joystick.h"
#include "zigbee.h"

void joystick_comm__send(void) {
  joystick__values_s data = joystick__get_value();
  printf("I %d %d\n", data.x, data.y);
  uint8_t message[Max_message_elemets] = {data.x, data.y, data.sw};
  zigbee__data_transfer(message, Max_message_elemets);
}