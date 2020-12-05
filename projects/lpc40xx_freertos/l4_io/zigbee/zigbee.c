
#include "zigbee.h"
#include "assert.h"
#include "gpio.h"
#include "lpc40xx.h"
#include "lpc_peripherals.h"
#include "stdio.h"

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

typedef enum zigbee_receive_state {
  Start_byte_state,
  Length_byte_state,
  Frame_bytes_state,
  Destination_address_state,
  Two_byte_address_state,
  Ignore_byte_state,
  Data_receive_state,
  Checksum_receive_state,
  Max_states,
} zigbee_receive_state;

typedef enum {
  MOSI_Pin_Mask = 1,
  MISO_Pin_Mask = 4,
  SSel_Pin_Mask = 14,
  Clk_Pin_Mask = 0,
  Attn_Pin_Mask = 6,
} SJ2_pins_to_connect_zigbee;

static zigbee_receive_state receive_state = Start_byte_state;
static uint64_t zigbee_joystick_address = 0x0013A20041C1A0A3;
static uint64_t zigbee_gun_address = 0x0013A20041C1A0D2;

QueueHandle_t zigbee__receiver_queue;

static uint8_t data_frame_header[Frame_header_size] = {0x7E, 00,   0xE,  0x10, 0x01, 00,   0x13, 0xA2, 00,
                                                       0x41, 0xB3, 0xCA, 0x57, 0xFF, 0xFE, 00,   00};

// static uint8_t data_frame_header[Frame_header_size] = {0x7E, 00,   0xE,  0x10, 0x01, 00,   0x13, 0xA2, 00,
//                                                        0x41, 0xC1, 0xA0, 0xD2, 0xFF, 0xFE, 00,   00};
static uint8_t calculate_checksum(uint8_t *data) {
  uint32_t sum = 0;
  uint8_t checksum = 0;
  uint16_t total_data_size = (data_frame_header[Length_byte_MSB] << 8) | (data_frame_header[Length_byte_LSB]);
  // printf("total data size from checksum calculation is %x\n", total_data_size);
  for (int i = 3; i < total_data_size + 3; i++) {
    if (i < Frame_header_size) {
      sum += data_frame_header[i];
    } else {
      sum += *data;
      data++;
    }
  }
  // printf("total sum from checksum is %lx\n", sum);
  sum = sum & 0xFF;
  checksum = (uint8_t)(0xFF - sum);
  return checksum;
}

static uint8_t calculate_checksum_receive(uint16_t sum) {
  sum &= 0xFF;
  return (uint8_t)(0xFF - sum);
}

static void zigbee_pin_configuration(void) {
  // Select SSP pin clock
  gpio__construct_with_function(GPIO__PORT_1, Clk_Pin_Mask, GPIO__FUNCTION_4);

  // Select SSP pin MOSI
  gpio__construct_with_function(GPIO__PORT_1, MOSI_Pin_Mask, GPIO__FUNCTION_4);

  // Select SSP pin MISO
  gpio__construct_with_function(GPIO__PORT_1, MISO_Pin_Mask, GPIO__FUNCTION_4);

  // Select CS pin to replicate original CS operation to monitor on Logic Analyzer
  gpio_s gpio_ssp_select_replicate = gpio__construct_as_output(GPIO__PORT_1, SSel_Pin_Mask);
  // Active low single needs be set by default
  gpio__set(gpio_ssp_select_replicate);

  // XBee module output this pin to get attention from master, when it has some valid data
  // to send
  gpio__construct_as_input(GPIO__PORT_0, Attn_Pin_Mask);
}

static void zigbee_data_receive_interrupt(void) {
  fprintf(stderr, "ISR\n");
  LPC_GPIOINT->IO0IntClr |= (1 << 6);
  xSemaphoreGiveFromISR(zigbee_spi_data_receive_sempahore, NULL);
}

static void zigbee__enable_spi_attn_interrupt(void) {
  lpc_peripheral__enable_interrupt(LPC_PERIPHERAL__GPIO, zigbee_data_receive_interrupt, "Zigbee SPI data received");
  LPC_GPIOINT->IO0IntEnF |= (1 << 6);
}

/*********************************************************************
 *********************PUBLIC FUNCTIONS********************************
 *********************************************************************/

void zigbee__cs(void) { LPC_GPIO1->CLR = (1 << SSel_Pin_Mask); }

void zigbee__ds(void) { LPC_GPIO1->SET = (1 << SSel_Pin_Mask); }

void zigbee__comm_init(void) {
  const uint32_t max_clock_khz = 1000;
  ssp2__initialize(max_clock_khz);
  zigbee_pin_configuration();
  zigbee__enable_spi_attn_interrupt();
  zigbee_spi_data_receive_sempahore = xSemaphoreCreateBinary();
  zigbee__cs();
}

void zigbee__data_transfer(uint8_t *data, size_t data_size) {
  data_size = data_size + data_frame_header[Length_byte_LSB];
  // printf("  Total data size frame headers is %x\n", data_frame_header[Length_byte_LSB]);
  data_frame_header[Length_byte_LSB] = data_size & 0xFF;
  data_frame_header[Length_byte_MSB] = (data_size >> 8) & 0xFF;
  uint8_t checksum = calculate_checksum(data);

  printf("\nChecksum value is %x", checksum);
  printf("  Total data size except checksum byte is %x   ", data_size);
  zigbee__cs();
  (void)ssp2__exchange_byte(data_frame_header[Start_byte]);
  (void)ssp2__exchange_byte(data_frame_header[Length_byte_MSB]);
  (void)ssp2__exchange_byte(data_frame_header[Length_byte_LSB]);

  // Iterate for all the frame bytes which are included in data size
  for (int i = Frame_type_byte; i < data_size + Frame_type_byte; i++) {
    if (i < Frame_header_size) {
      (void)ssp2__exchange_byte(data_frame_header[i]);
      printf(" %x\t", data_frame_header[i]);
    } else if (i < data_size + Frame_type_byte) {
      (void)ssp2__exchange_byte(*data);
      printf(" %x\t", *data);
      data++;
    }
  }
  (void)ssp2__exchange_byte(checksum);
  zigbee__ds();
  // Resetting the frame length parameter in frame header
  data_frame_header[Length_byte_LSB] = 0xE;
  data_frame_header[Length_byte_MSB] = 0x0;
}

void zigbee__data_parcer(uint8_t data) {
  static uint8_t bytes_remaining_to_receive;
  static uint16_t data_length;
  static uint16_t data_sum;
  static uint64_t destination_address;
  static uint16_t message_length;
  static uint64_t message;
  static uint8_t checksum;
  switch (receive_state) {
  case Start_byte_state:
    if (data == 0x7E) {
      receive_state = Length_byte_state;
      bytes_remaining_to_receive = 2;
    }
    break;
  case Length_byte_state:
    if (--bytes_remaining_to_receive > 0) {
      data_length = (data_length << 8) | data;
    } else {
      data_length = (data_length << 8) | data;
      message_length = data_length - 0xC;
      // For this application, message length is of 3 bytes. Thus, do error check
      // assert(message_length == Max_message_elemets); // "Message length of received frame not matching");
      receive_state = Frame_bytes_state;
      bytes_remaining_to_receive = 1;
    }
    break;
  case Frame_bytes_state:
    if (--bytes_remaining_to_receive > 0) {
      data_sum = +data;
    } else {
      data_sum = +data;
      receive_state = Destination_address_state;
      bytes_remaining_to_receive = 8;
    }
    break;
  case Destination_address_state:
    if (--bytes_remaining_to_receive > 0) {
      data_sum += data;
      destination_address = (destination_address << 8) | data;
    } else {
      data_sum += data;
      destination_address = (destination_address << 8) | data;

      if (destination_address == zigbee_joystick_address) {
        receive_state = Two_byte_address_state;
        bytes_remaining_to_receive = 2;
      } else {
        receive_state = Two_byte_address_state;
        bytes_remaining_to_receive = 2;
        // printf("XX\n");
      }
    }
    break;
  case Two_byte_address_state:
    if (--bytes_remaining_to_receive > 0) {
      data_sum += data;
    } else {
      data_sum += data;
      receive_state = Ignore_byte_state;
      bytes_remaining_to_receive = 1;
    }
    break;
  case Ignore_byte_state:
    if (--bytes_remaining_to_receive > 0) {
      data_sum += data;
    } else {
      data_sum += data;
      receive_state = Data_receive_state;
      bytes_remaining_to_receive = message_length;
    }
    break;
  case Data_receive_state:
    if (--bytes_remaining_to_receive > 0) {
      data_sum += data;
      if (message_length == Max_message_elemets)
        zigbee_message[bytes_remaining_to_receive] = data;
      else
        message = (message << 8) | data;

    } else {
      data_sum += data;
      if (message_length == Max_message_elemets)
        zigbee_message[bytes_remaining_to_receive] = data;
      else
        message = (message << 8) | data;
      checksum = calculate_checksum_receive(data_sum);
      bytes_remaining_to_receive = 1;
      receive_state = Checksum_receive_state;
    }
    break;
  case Checksum_receive_state:
    if (data == checksum) {
      data_length = 0;
      fprintf(stderr, "Y\n");
    } else {
      fprintf(stderr, "X %x\n", checksum);
    }
    receive_state = Start_byte_state;
    break;
  case Max_states:
    receive_state = Start_byte_state;
    break;
  }
}