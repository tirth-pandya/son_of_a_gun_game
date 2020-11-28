
#pragma once
#include "FreeRTOS.h"
#include "queue.h"
#include "uart.h"

typedef enum zigbee_comm_message_element {
  Button_press,
  Y_coord,
  X_coord,
  Max_message_elemets,
} zigbee_comm_message_element;

volatile uint8_t zigbee_message[Max_message_elemets];
/**
 * @brief
 */
void zigbee__comm_init(uart_e uart, const uint32_t uart_baud_rate);

void zigbee__data_transfer(uint8_t *data, size_t data_size);

void zigbee__data_receiver(uint8_t data);
bool zigbee__receive_data(uint8_t *message);