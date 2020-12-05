#include "acceleration.h"
#include "gun.h"
#include "zigbee.h"

void gun_comm__send_data(void) {

  gun_param gun_coordinates = gun__get_current_param(500, 500);
  uint8_t message[Max_message_elemets] = {gun_coordinates.x_coord, gun_coordinates.y_coord, gun_coordinates.button};
  zigbee__data_transfer(message, Max_message_elemets);
}