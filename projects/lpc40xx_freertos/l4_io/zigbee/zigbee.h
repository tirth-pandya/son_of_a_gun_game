
#pragma once
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "ssp2.h"
#include "stdbool.h"
#include "stdio.h"

typedef enum zigbee_comm_message_element {
  Button_press,
  Y_coord,
  X_coord,
  Max_message_elemets,
} zigbee_comm_message_element;

volatile uint8_t zigbee_message[Max_message_elemets];

xSemaphoreHandle zigbee_spi_data_receive_sempahore;
/**
 * @brief
 */
void zigbee__comm_init(bool is_receiver);

/**
 * @brief
 */
void zigbee__data_transfer(uint8_t *data, size_t data_size);

/**
 * @brief
 */
void zigbee__data_parcer(uint8_t data);

/**
 * @brief
 */
void zigbee__cs(void);

/**
 * @brief
 */
void zigbee__ds(void);