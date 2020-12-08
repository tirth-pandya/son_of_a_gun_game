#include "gun_comm.h"
#include "acceleration.h"
#include "gun.h"
#include "zigbee.h"

uint8_t message[Max_message_elemets];
static uint8_t prev_x_value;
static uint8_t prev_y_value;

void gun_comm__send_data(void) {

  gun_param gun_coordinates = gun__get_current_param(500, 650);
  if (message[X_coord] != 1) {
    prev_x_value = message[Button_press];
    prev_y_value = message[Y_coord];
    message[Y_coord] = gun_coordinates.y_coord;
    message[Button_press] = gun_coordinates.x_coord;
  } else {
    message[Y_coord] = prev_y_value;
    message[Button_press] = prev_x_value;
  }
  zigbee__data_transfer(message, Max_message_elemets);
  printf("data is %d %d %d\n", message[Button_press], message[Y_coord], message[X_coord]);
  gun_com__clear_gunShotValue();
}

void gun_com__set_gunShotValue(void) { message[X_coord] = 1; }

void gun_com__clear_gunShotValue(void) { message[X_coord] = 0; }