
/**
 * @brief
 */
void zigbee__comm_init(uart_e uart, const uint32_t uart_baud_rate);

void zigbee__data_transfer(uint8_t *data, size_t data_size);