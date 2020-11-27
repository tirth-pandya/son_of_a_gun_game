#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "board_io.h"
#include "common_macros.h"
#include "periodic_scheduler.h"
#include "sj2_cli.h"

#include "graphics.h"
#include "led_matrix.h"

#include "acceleration.h"
#include "delay.h"
#include "ff.h"
#include "joystick.h"
#include "object_tracking.h"
#include "shapes.h"

static const data_size ALL_LED = 0x0000FFFFFFFF0000;

acceleration__axis_data_s sensor_data;
bool sensor_state;

static void joystick_task(void *p) {
  joystick__values_s joystick_val = {0};
  gpio_s x = {0, 25};
  gpio_s y = {1, 30};
  gpio_s s_k = {1, 31};
  joystick__initialize(x, y, s_k);

  while (1) {
    joystick_val = joystick__get_value();
    // led_matrix__clear_data_buffer();
    // led_matrix__set_pixel(joystick_val.y, joystick_val.x, RED);
    char my_ch[2] = {"x"};
    print_char(my_ch, joystick_val.y, 63 - joystick_val.x, 7);
    // printf("X axis : %d,\tY axis : %d\n", joystick_val.x, joystick_val.y);
    graphics__turn_on_all_leds(GREEN);
    vTaskDelay(10);
  }
}
static void acceleration_task(void *p) {
  sensor_state = acceleration__init();
  if (!sensor_state) {
    acceleration__init();
  }
  acceleration__axis_data_s acceleration_rx_data;
  uint8_t led_pt_x = 0, led_pt_y = 0;
  while (1) {
    acceleration_rx_data = acceleration__get_averaged_data(20, 100);
    led_pt_x = acceleration_rx_data.x;
    led_pt_y = acceleration_rx_data.y;
    led_matrix__clear_data_buffer();
    led_matrix__set_pixel(led_pt_x, led_pt_y, RED);
    vTaskDelay(10);
  }
}
static void graphics_task(void *p);
static void display_task(void *p);

int main(void) {
  // create_blinky_tasks();
  // create_uart_task();

  xTaskCreate(display_task, "display", 1024 / sizeof(void *), NULL, PRIORITY_HIGH, NULL);
  xTaskCreate(graphics_task, "graphics", 4096 / sizeof(void *), NULL, PRIORITY_LOW, NULL);
  // xTaskCreate(acceleration_task, "read_acc", 2048 / sizeof(void *), NULL, PRIORITY_LOW, NULL);
  // xTaskCreate(joystick_task, "read_joystick", 2048 / sizeof(void *), NULL, PRIORITY_LOW, NULL);
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
  initialize_object_details();

  uint8_t hit = 1;
  while (1) {
    randomizer_objects();
    led_matrix__clear_data_buffer();
    // graphics__turn_on_all_leds(1);
    draw_from_structure();
    detect_click(hit);
    vTaskDelay(50);
  }
}

#ifdef DEF_TASK
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
* /
#endif