#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "board_io.h"
#include "common_macros.h"
#include "periodic_scheduler.h"
#include "sj2_cli.h"

#include "uart.h"
#include <stdlib.h>
#include <string.h>

// 'static' to make these functions 'private' to this file
static void create_blinky_tasks(void);
static void create_uart_task(void);
static void blink_task(void *params);
static void uart_task(void *params);

static int8_t Send_buf[8] = {0}; // The MP3 player undestands orders in a 8 int string
                                 // 0X7E FF 06 command 00 00 00 EF;(if command =01 next song order)
#define NEXT_SONG 0X01
#define PREV_SONG 0X02
#define CMD_PLAY_W_INDEX 0X03 // DATA IS REQUIRED (number of song)
#define VOLUME_UP_ONE 0X04
#define VOLUME_DOWN_ONE 0X05
#define CMD_SET_VOLUME 0X06 // DATA IS REQUIRED (number of volume from 0 up to 30(0x1E))
#define SET_DAC 0X17
#define CMD_PLAY_WITHVOLUME 0X22 // data is needed  0x7E 06 22 00 xx yy EF;(xx volume)(yy number of song)
#define CMD_SEL_DEV 0X09         // SELECT STORAGE DEVICE, DATA IS REQUIRED
#define DEV_TF 0X02              // HELLO,IM THE DATA REQUIRED
#define SLEEP_MODE_START 0X0A
#define SLEEP_MODE_WAKEUP 0X0B
#define CMD_RESET 0X0C // CHIP RESET
#define CMD_PLAY 0X0D  // RESUME PLAYBACK
#define CMD_PAUSE 0X0E // PLAYBACK IS PAUSED
#define CMD_PLAY_WITHFOLDER                                                                                            \
  0X0F // DATA IS NEEDED, 0x7E 06 0F 00 01 02 EF;(play the song with the directory \01\002xxxxxx.mp3
#define STOP_PLAY 0X16
#define PLAY_FOLDER 0X17   // data is needed 0x7E 06 17 00 01 XX EF;(play the 01 folder)(value xx we dont care)
#define SET_CYCLEPLAY 0X19 // data is needed 00 start; 01 close
#define SET_DAC 0X17       // data is needed 00 start DAC OUTPUT;01 DAC no output

void sendCommand_card() {
  Send_buf[0] = 0x7e;
  Send_buf[1] = 0xff;
  Send_buf[2] = 0x06;
  Send_buf[3] = 0x09;
  Send_buf[4] = 0x00;
  Send_buf[5] = 0x00;
  Send_buf[6] = 0x02;
  Send_buf[7] = 0xef;
  for (uint8_t i = 0; i < 8; i++)
    uart__polled_put(UART__3, Send_buf[i]);
}

void sendCommand_play() {
  Send_buf[0] = 0x7e;
  Send_buf[1] = 0xff;
  Send_buf[2] = 0x06;
  Send_buf[3] = 0x22;
  Send_buf[4] = 0x00;
  Send_buf[5] = 0x1e;
  Send_buf[6] = 0x01;
  Send_buf[7] = 0xef;
  for (uint8_t i = 0; i < 8; i++)
    uart__polled_put(UART__3, Send_buf[i]);
}
void sendCommand(int8_t command, int16_t dat) {
  Send_buf[0] = 0x7e;               // starting byte
  Send_buf[1] = 0xff;               // version
  Send_buf[2] = 0x06;               // the number of bytes of the command without starting byte and ending byte
  Send_buf[3] = command;            //
  Send_buf[4] = 0x00;               // 0x00 = no feedback, 0x01 = feedback
  Send_buf[5] = (int8_t)(dat >> 8); // datah
  Send_buf[6] = (int8_t)(dat);      // datal
  Send_buf[7] = 0xef;               // ending byte
  for (uint8_t i = 0; i < 8; i++)
    uart__polled_put(UART__3, Send_buf[i]);
}

void sendCommand1(void) {
  Send_buf[0] = 0x7e;
  Send_buf[1] = 0x03;
  Send_buf[2] = 0x33;
  Send_buf[3] = 0x00;
  Send_buf[4] = 0xef;
  for (uint8_t i = 0; i < 5; i++)
    uart__polled_put(UART__3, Send_buf[i]);
}

void sendCommand2(void) {
  Send_buf[0] = 0x7e;
  Send_buf[1] = 0x03;
  Send_buf[2] = 0x35;
  Send_buf[3] = 0x01;
  Send_buf[4] = 0xef;
  printf("sC2 \n");
  for (uint8_t i = 0; i < 5; i++)
    uart__polled_put(UART__3, Send_buf[i]);
}
void sendCommand3(void) {
  Send_buf[0] = 0x7e;
  Send_buf[1] = 0x02;
  Send_buf[2] = 0x01;
  Send_buf[3] = 0xef;
  printf("sC3\n");
  for (uint8_t i = 0; i < 4; i++)
    uart__polled_put(UART__3, Send_buf[i]);
  printf("completed sC3\n");
}
void sendCommand4(void) {
  Send_buf[0] = 0x7e;
  Send_buf[1] = 0x02;
  Send_buf[2] = 0x10;
  Send_buf[3] = 0xef;
  for (uint8_t i = 0; i < 4; i++)
    uart__polled_put(UART__3, Send_buf[i]);
}
void send_task(void *p) {
  while (1) {
    sendCommand_card();
    sendCommand_play();
    vTaskDelay(5000);
  }
}
int main(void) {
  //  create_blinky_tasks();
  // create_uart_task();

  // If you have the ESP32 wifi module soldered on the board, you can try uncommenting this code
  // See esp32/README.md for more details
  // uart3_init();                                                                     // Also include:  uart3_init.h
  // xTaskCreate(esp32_tcp_hello_world_task, "uart3", 1000, NULL, PRIORITY_LOW, NULL); // Include esp32_task.h

  puts("Starting RTOS");

  uart__init(UART__3, 96 * 1000 * 1000, 9600);
  gpio__construct_with_function(GPIO__PORT_4, 28, GPIO__FUNCTION_2);
  gpio__construct_with_function(GPIO__PORT_4, 29, GPIO__FUNCTION_2);
  // sendCommand(0x08, 0x0003);
  // sendCommand(CMD_SET_VOLUME, 25);
  // sendCommand(0x03, 0x0001);
  // sendCommand(0x08, 0x0001);
  // sendCommand(CMD_SET_VOLUME, 25);

  printf("initialized..");
  // sendCommand2();
  // sendCommand1();
  // sendCommand3();
  xTaskCreate(send_task, "uart", 2048 / sizeof(void *), NULL, PRIORITY_LOW, NULL);
  // printf("\nI did it");
  // while (1)
  //   ;

  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}

static void create_blinky_tasks(void) {
  /**
   * Use '#if (1)' if you wish to observe how two tasks can blink LEDs
   * Use '#if (0)' if you wish to use the 'periodic_scheduler.h' that will spawn 4 periodic tasks, one for each LED
   */
#if (1)
  // These variables should not go out of scope because the 'blink_task' will reference this memory
  static gpio_s led0, led1;

  led0 = board_io__get_led0();
  led1 = board_io__get_led1();

  xTaskCreate(blink_task, "led0", configMINIMAL_STACK_SIZE, (void *)&led0, PRIORITY_LOW, NULL);
  xTaskCreate(blink_task, "led1", configMINIMAL_STACK_SIZE, (void *)&led1, PRIORITY_LOW, NULL);
#else
  const bool run_1000hz = true;
  const size_t stack_size_bytes = 2048 / sizeof(void *); // RTOS stack size is in terms of 32-bits for ARM M4 32-bit CPU
  periodic_scheduler__initialize(stack_size_bytes, !run_1000hz); // Assuming we do not need the high rate 1000Hz task
  UNUSED(blink_task);
#endif
}

static void create_uart_task(void) {
  // It is advised to either run the uart_task, or the SJ2 command-line (CLI), but not both
  // Change '#if (0)' to '#if (1)' and vice versa to try it out
#if (0)
  // printf() takes more stack space, size this tasks' stack higher
  xTaskCreate(uart_task, "uart", (512U * 8) / sizeof(void *), NULL, PRIORITY_LOW, NULL);
#else
  sj2_cli__init();
  UNUSED(uart_task); // uart_task is un-used in if we are doing cli init()
#endif
}

static void blink_task(void *params) {
  const gpio_s led = *((gpio_s *)params); // Parameter was input while calling xTaskCreate()

  // Warning: This task starts with very minimal stack, so do not use printf() API here to avoid stack overflow
  while (true) {
    gpio__toggle(led);
    vTaskDelay(500);
  }
}

// This sends periodic messages over printf() which uses system_calls.c to send them to UART0
static void uart_task(void *params) {
  TickType_t previous_tick = 0;
  TickType_t ticks = 0;

  while (true) {
    // This loop will repeat at precise task delay, even if the logic below takes variable amount of ticks
    vTaskDelayUntil(&previous_tick, 2000);

    /* Calls to fprintf(stderr, ...) uses polled UART driver, so this entire output will be fully
     * sent out before this function returns. See system_calls.c for actual implementation.
     *
     * Use this style print for:
     *  - Interrupts because you cannot use printf() inside an ISR
     *    This is because regular printf() leads down to xQueueSend() that might block
     *    but you cannot block inside an ISR hence the system might crash
     *  - During debugging in case system crashes before all output of printf() is sent
     */
    ticks = xTaskGetTickCount();
    fprintf(stderr, "%u: This is a polled version of printf used for debugging ... finished in", (unsigned)ticks);
    fprintf(stderr, " %lu ticks\n", (xTaskGetTickCount() - ticks));

    /* This deposits data to an outgoing queue and doesn't block the CPU
     * Data will be sent later, but this function would return earlier
     */
    ticks = xTaskGetTickCount();
    printf("This is a more efficient printf ... finished in");
    printf(" %lu ticks\n\n", (xTaskGetTickCount() - ticks));
  }
}
