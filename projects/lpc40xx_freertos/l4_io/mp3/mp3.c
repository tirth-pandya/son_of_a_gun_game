#include "mp3.h"
#include "gpio.h"
#include "uart.h"

const uint32_t default_bg_duration = 6000;
const uint32_t gunshot_duration = 500;
static uint8_t mp3_uart_buffer[8];

bool mp3__init() {
  bool status = false;
  int mp3_baud_rate = 9600;
  uart__init(UART__3, clock__get_peripheral_clock_hz(), mp3_baud_rate);
  gpio__construct_with_function(GPIO__PORT_0, 1, GPIO__FUNCTION_2);
  gpio__construct_with_function(GPIO__PORT_0, 0, GPIO__FUNCTION_2);
  return status = true;
}

bool mp3__send_command(uint8_t command, uint16_t data) {
  bool status = false;
  uint8_t data_ub = (uint8_t)(data >> 8);
  uint8_t data_lb = (uint8_t)(data);
  mp3_uart_buffer[0] = 0x7e;
  mp3_uart_buffer[1] = 0xff;
  mp3_uart_buffer[2] = 0x06;
  mp3_uart_buffer[3] = command;
  mp3_uart_buffer[4] = 0x00;
  mp3_uart_buffer[5] = data_ub;
  mp3_uart_buffer[6] = data_lb;
  mp3_uart_buffer[7] = 0xef;

  for (int i = 0; i < 8; i++) {
    uart__polled_put(UART__3, mp3_uart_buffer[i]);
  }
  return status = true;
}

void update_mp3_details(MP3_SOUNDS sound_file, uint32_t sound_duration) {
  mp3_details.mp3_to_play = sound_file;
  mp3_details.mp3_duration = sound_duration;
}