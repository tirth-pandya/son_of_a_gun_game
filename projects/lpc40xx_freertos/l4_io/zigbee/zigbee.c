
#include "gpio.h"
#include "stdio.h"
#include "uart.h"

enum API_data_frame_header {
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
  Frame_header_size,
};

static uint8_t data_frame_header[Frame_header_size] = {0x7E, 00,   0xE,  0x10, 0x01, 00,   0x13, 0xA2, 00,
                                                       0x41, 0xB3, 0xCA, 0x57, 0xFF, 0xFE, 00,   00};

static uint8_t calculate_checksum(uint8_t *data) {
  uint32_t sum = 0;
  uint8_t checksum = 0;
  uint16_t total_data_size = (data_frame_header[Length_byte_MSB] << 8) | (data_frame_header[Length_byte_LSB]);
  printf("total data size from checksum calculation is %x\n", total_data_size);
  for (int i = 3; i < total_data_size + 3; i++) {
    if (i < Frame_header_size) {
      sum += data_frame_header[i];
    } else {
      sum += *data;
      data++;
    }
  }
  printf("total sum from checksum is %lx\n", sum);
  sum = sum & 0xFF;
  checksum = (uint8_t)(0xFF - sum);
  return checksum;
}

void zigbee_comm_init(uart_e uart, const uint32_t uart_baud_rate) {
  const uint32_t peripheral_clock = clock__get_peripheral_clock_hz();
  uart__init(UART__2, peripheral_clock, uart_baud_rate);

  gpio__construct_with_function(GPIO__PORT_2, 8, GPIO__FUNCTION_2);
  gpio__construct_with_function(GPIO__PORT_2, 9, GPIO__FUNCTION_2);
}

void zigbee_data_transfer(uint8_t *data, size_t data_size) {
  data_size = data_size + data_frame_header[Length_byte_LSB];
  printf("  Total data size frame headers is %x\n", data_frame_header[Length_byte_LSB]);
  data_frame_header[Length_byte_LSB] = data_size & 0xFF;
  data_frame_header[Length_byte_MSB] = (data_size >> 8) & 0xFF;
  uint8_t checksum = calculate_checksum(data);

  printf("Checksum value is %x", checksum);
  printf("  Total data size except checksum byte is %x\n", data_size);
  (void)uart__polled_put(UART__2, data_frame_header[Start_byte]);
  (void)uart__polled_put(UART__2, data_frame_header[Length_byte_MSB]);
  (void)uart__polled_put(UART__2, data_frame_header[Length_byte_LSB]);

  // Iterate for all the frame bytes which are included in data size
  for (int i = Frame_type_byte; i < data_size + Frame_type_byte; i++) {
    if (i < Frame_header_size) {
      while (!(uart__polled_put(UART__2, data_frame_header[i]))) {
      }
      printf("Sent %x\t", data_frame_header[i]);
    } else if (i < data_size + Frame_type_byte) {
      while (!(uart__polled_put(UART__2, *data))) {
      }
      printf("Sent %x\t", *data);
      data++;
    }
  }
  (void)uart__polled_put(UART__2, checksum);
  data_frame_header[Length_byte_LSB] = 0xE;
  data_frame_header[Length_byte_MSB] = 0x0;
}
