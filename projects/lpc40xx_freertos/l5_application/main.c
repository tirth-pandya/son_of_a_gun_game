#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "board_io.h"
#include "common_macros.h"
#include "periodic_scheduler.h"
#include "sj2_cli.h"
#include "uart.h"
#include "zigbee.h"

#include "graphics.h"
#include "led_matrix.h"

#include "acceleration.h"
#include "alphabets.h"
#include "delay.h"
#include "ff.h"
#include "joystick.h"
#include "object_tracking.h"
#include "joystick_comm.h"
#include "shapes.h"

acceleration__axis_data_s sensor_data;
bool sensor_state;

SemaphoreHandle_t controller_data_update_mutex;

static void joystick_task(void *p) {
  gpio_s x = {0, 25};
  gpio_s y = {1, 30};
  gpio_s s_k = {1, 31};
  joystick__initialize(x, y, s_k);
  zigbee__cs();
  while (1) {

    // joystick_val = joystick__get_value();
    // led_matrix__clear_data_buffer();
    // led_matrix__set_pixel(joystick_val.y, joystick_val.x, RED);
    // char my_ch[2] = {"x"};
    // print_char(my_ch, joystick_val.y, 63 - joystick_val.x, 7);
    // printf("X axis : %d,\tY axis : %d\n", joystick_val.x, joystick_val.y);
    // graphics__turn_on_all_leds(GREEN);
    joystick_comm__send();
    vTaskDelay(30);
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

static void receive_zigbee_task(void *p);

int main(void) {
  // create_blinky_tasks();
  // create_uart_task();
  controller_data_update_mutex = xSemaphoreCreateMutex();

  zigbee__comm_init();
  // xTaskCreate(receive_zigbee_task, "zigbee_receive", 2048 / sizeof(void *), NULL, PRIORITY_HIGH, NULL);
  // xTaskCreate(display_task, "display", 1024 / sizeof(void *), NULL, PRIORITY_HIGH, NULL);
  // xTaskCreate(graphics_task, "graphics", 1024 / sizeof(void *), NULL, PRIORITY_LOW, NULL);
  // xTaskCreate(acceleration_task, "read_acc", 2048 / sizeof(void *), NULL, PRIORITY_LOW, NULL);
  xTaskCreate(joystick_task, "read_joystick", 2048 / sizeof(void *), NULL, PRIORITY_LOW, NULL);
  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails
  return 0;
}

void receive_zigbee_task(void *p) {
  uint8_t message = 0;
  zigbee__cs();
  while (1) {
    if (xSemaphoreTake(zigbee_spi_data_receive_sempahore, portMAX_DELAY)) {
      const uint8_t dummy_MOSI_data = 0xFF;
      gpio_s gpio_attn = gpio__construct(0, 6);
      printf("rece ");
      while (!gpio__get(gpio_attn)) {
        message = ssp2__exchange_byte(dummy_MOSI_data);
        printf("%x ", message);
        zigbee__data_parcer(message);
      }
      printf("\n");
    }
  }
}
void display_task(void *p) {
  led_matrix_init();

  while (1) {
    // char test123 = "abc";
    // print_char(test123, 32, 32, BLUE);
    // shape_update(10, 20, test, 3, FRIEND);
    led_matrix__update_display();

    // print_char(test, 9, 5, BLUE);
    // print_char(test1, 16, 2, GREEN);
    vTaskDelay(2);
  }
}

void graphics_task(void *p) {
  graphics__turn_off_all_leds();
  initialize_object_details();
  // char test = "test";

  // void (*draw_enemy_pointer)(void);
  // draw_enemy_pointer = &draw_enemy;

  uint8_t hit = 1;
  while (1) {
    // print_char(test, 9, 5, BLUE);

    // shape_update(10, 20, a1, GREEN, FRIEND);
    // shape_update(10, 20, a2, RED, FRIEND);
    // shape_update(10, 20, a3, BLUE, FRIEND);

    randomizer_objects();
    led_matrix__clear_data_buffer();
    // draw_enemy_pointer();
    draw_from_structure();
    detect_click(hit);
    collision_detection();
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

  void send_zigbee_task(void *p) {
    while (1) {
      uint8_t data[] = {0x68, 0x65, 0x6C, 0x6C, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F};
      uint8_t size = sizeof(data);
      zigbee__data_transfer(data, size);
      // while (!(uart_lab__polled_put(UART__2, write_byte))) {
      // }
      // printf("The data %X is written successfully\n", write_byte);
      vTaskDelay(2000);
    }
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
#endif