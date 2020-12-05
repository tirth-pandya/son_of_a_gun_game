#include "acceleration.h"
#include "gun.h"
#include "zigbee.h"

uint8_t message[Max_message_elemets];

void gun_comm__send_data(void) {

  gun_param gun_coordinates = gun__get_current_param(500, 650);
  message[X_coord] = gun_coordinates.button;
  message[Y_coord] = gun_coordinates.y_coord;
  message[Button_press] = gun_coordinates.x_coord;
  zigbee__data_transfer(message, Max_message_elemets);
}

void gun_comm__send_gunFire(void) {
  message[X_coord] = 1;
  printf("got gun shot\n");
  zigbee__data_transfer(message, Max_message_elemets);
}