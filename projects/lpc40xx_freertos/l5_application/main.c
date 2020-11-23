#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "board_io.h"
#include "common_macros.h"
#include "periodic_scheduler.h"
#include "sj2_cli.h"

#include "graphics.h"
#include "led_matrix.h"

static const data_size ALL_LED = 0x0000FFFFFFFF0000;

// 'static' to make these functions 'private' to this file
static void create_blinky_tasks(void);
static void create_uart_task(void);
static void blink_task(void *params);
static void uart_task(void *params);

static void graphics_task(void *p);
static void display_task(void *p);

int main(void) {
  // create_blinky_tasks();
  // create_uart_task();

  xTaskCreate(display_task, "display", 1024 / sizeof(void *), NULL, PRIORITY_HIGH, NULL);
  xTaskCreate(graphics_task, "graphics", 1024 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);

  // If you have the ESP32 wifi module soldered on the board, you can try uncommenting this code
  // See esp32/README.md for more details
  // uart3_init();                                                                     // Also include:  uart3_init.h
  // xTaskCreate(esp32_tcp_hello_world_task, "uart3", 1000, NULL, PRIORITY_LOW, NULL); // Include esp32_task.h

  puts("Starting RTOS");
  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}

void display_task(void *p) {
  led_matrix_init();

  while (1) {
    led_matrix__update_display();

    // print_char(test, 9, 5, BLUE);
    // print_char(test1, 16, 2, GREEN);
    vTaskDelay(1);
  }
}

void graphics_task(void *p) {
  graphics__turn_off_all_leds();
  int First_row_start = 0;
  int First_row_end = 1;
  int sec_row_start = 22;
  int sec_row_end = 44;
  int third_row_start = 44;
  int third_row_end = 64;

  while (1) {
    // graphics__turn_o
    char test[11] = {"abcdefghij"};
    char test1[11] = {"klmnopqrst"};
    char test2[11] = {"uvwxyz"};
    char test3[11] = {"0123456789"};
    // print_char(test, 10, 2, CYAN);
    for (int i = 1; i < 8; i++) {
      // led_matrix__clear_data_buffer();

      print_char(test, 0, 0, i);
      print_char(test1, 6, 0, i);
      print_char(test2, 12, 0, i);
      print_char(test3, 18, 0, i);

      vTaskDelay(500);
      graphics__turn_off_all_leds();
    }
    // led_matrix__fill_data_buffer_till_row(ALL_LED, First_row_start, First_row_end, GREEN);
    // vTaskDelay(1000);n_all_leds(RED);
    // vTaskDelay(500);
    // graphics__turn_off_all_leds();
    // vTaskDelay(200);
    // graphics__turn_on_all_leds(GREEN);
    //   led_matrix__set_row_data(row, GREEN, ALL_LED);
    // led_matrix__fill_data_buffer_till_row(ALL_LED, First_row_start, First_row_end, GREEN);
    // graphics_print_test_row();
    // led_matrix__fill_data_buffer_till_row(ALL_LED, third_row_start, third_row_end, LIME);
    // vTaskDelay(1000);
  }
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
