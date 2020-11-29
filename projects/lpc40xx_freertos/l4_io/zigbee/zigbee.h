
#pragma once
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "uart.h"

#include "ssp2.h"

xSemaphoreHandle zigbee_spi_data_receive_sempahore;
/**
 * @brief
 */
void zigbee__comm_init(uart_e uart, const uint32_t uart_baud_rate);

void zigbee__data_transfer(uint8_t *data, size_t data_size);

void zigbee__data_receiver(uint8_t data);
bool zigbee__receive_data(uint8_t *message);

void zigbee__cs(void);

void zigbee__ds(void);