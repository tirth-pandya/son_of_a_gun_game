
#include "gpio.h"
#include "stdio.h"
#include "uart.h"
#include "lpc_peripherals.h"
#include "lpc40xx.h"
#include "assert.h"

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

static uart_e zigbee_uart;

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

static uart3_receive_interrupt(void) {
  const uint8_t interrupt_id_mask = (0b111);
  const uint8_t interrupt_pending_bit = (1 << 0);
  const uint8_t lsr_reg_received_data_ready_bit_mask = (1 << 0);
  uint8_t interrupt_id;
  // Check whether there is pending interrupt corresponding to UART3
  if (!(LPC_UART3->IIR & interrupt_pending_bit))
    interrupt_id = ((LPC_UART3->IIR >> 1) & interrupt_id_mask);
  else {
    fprintf(stderr, "UART Interrupt occured due to unconfigured UART cannel, habe look into it");
    assert(0);
  }

  if ((interrupt_id == 2) && (LPC_UART3->LSR & lsr_reg_received_data_ready_bit_mask)) {
    const char byte = LPC_UART3->RBR;
    xQueueSendFromISR(uart_rx_queue, &byte, NULL);
    // printf("I am in ISR, Just put %X data in queue\n", byte);
  }
}

/*********************************************************************
 *********************PUBLIC FUNCTIONS********************************
 *********************************************************************/

void zigbee__comm_init(uart_e uart, const uint32_t uart_baud_rate) {
  const uint32_t peripheral_clock = clock__get_peripheral_clock_hz();
  zigbee_uart = uart;
  uart__init(UART__3, peripheral_clock, uart_baud_rate);

  gpio__construct_with_function(GPIO__PORT_4, 28, GPIO__FUNCTION_2); // UART_TX
  gpio__construct_with_function(GPIO__PORT_4, 29, GPIO__FUNCTION_2); // UART_RX
}

void zigbee__data_transfer(uint8_t *data, size_t data_size) {
  data_size = data_size + data_frame_header[Length_byte_LSB];
  printf("  Total data size frame headers is %x\n", data_frame_header[Length_byte_LSB]);
  data_frame_header[Length_byte_LSB] = data_size & 0xFF;
  data_frame_header[Length_byte_MSB] = (data_size >> 8) & 0xFF;
  uint8_t checksum = calculate_checksum(data);

  printf("Checksum value is %x", checksum);
  printf("  Total data size except checksum byte is %x\n", data_size);
  (void)uart__polled_put(UART__3, data_frame_header[Start_byte]);
  (void)uart__polled_put(UART__3, data_frame_header[Length_byte_MSB]);
  (void)uart__polled_put(UART__3, data_frame_header[Length_byte_LSB]);

  // Iterate for all the frame bytes which are included in data size
  for (int i = Frame_type_byte; i < data_size + Frame_type_byte; i++) {
    if (i < Frame_header_size) {
      while (!(uart__polled_put(UART__3, data_frame_header[i]))) {
      }
      printf("Sent %x\t", data_frame_header[i]);
    } else if (i < data_size + Frame_type_byte) {
      while (!(uart__polled_put(UART__3, *data))) {
      }
      printf("Sent %x\t", *data);
      data++;
    }
  }
  (void)uart__polled_put(UART__3, checksum);
  data_frame_header[Length_byte_LSB] = 0xE;
  data_frame_header[Length_byte_MSB] = 0x0;
}


void zigbee__uart_enable_interrupt(void) {
  if(zigbee_uart == UART__3) {
    const uint32_t dlab_bit_mask = (1 << 7);
    const uint8_t receive_data_enable_interrupt_bit_mask = (1 << 0);
    LPC_UART3->LCR &= ~(dlab_bit_mask);
    lpc_peripheral__enable_interrupt(LPC_PERIPHERAL__UART3, uart3_receive_interrupt, "UART3 DATA RECEIVED");
    LPC_UART3->IER  |= (receive_data_enable_interrupt_bit_mask);
  }
  else {
    fprintf(stderr,"zigbee is not on UART3, please reverify");
    assert(1);
  }
}