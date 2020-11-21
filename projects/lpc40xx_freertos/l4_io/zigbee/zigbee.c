
#include "uart.h"
#include "gpio.h"

typedef enum API_data_frame{
    Start_byte,
    Length_byte_MSB,
    Length_byte_LSB,
    Frame_type_byte,
    Frame_ID_byte,
    Destination_address_bytes,
    Two_byte_dest_addr_MSB = 13,
    Two_byte_dest_addr_LSB,
    Broadcast_radius_byte,
    Options_byte,
};

void zigbee_comm_init(uart_e uart, const uint32_t uart_baud_rate) {
    const uint32_t peripheral_clock = clock__get_peripheral_clock_hz();
    uart__init(UART__2, peripheral_clock, uart_baud_rate);

    gpio__construct_with_function(GPIO__PORT_2, 8, GPIO__FUNCTION_2);
    gpio__construct_with_function(GPIO__PORT_2, 9, GPIO__FUNCTION_2);
}

void zigbee_data_transfer(uint8_t *data, size_t data_size) {

}

