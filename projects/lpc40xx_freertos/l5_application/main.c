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
#include "gun.h"
#include "gun_comm.h"
#include "joystick.h"
#include "joystick_comm.h"
#include "object_tracking.h"
#include "shapes.h"

acceleration__axis_data_s sensor_data;
bool sensor_state;

SemaphoreHandle_t controller_data_update_mutex;
extern volatile uint8_t zigbee_joystick_message[Max_message_elemets];
extern volatile uint8_t zigbee_gun_message[Max_message_elemets];

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

void gun_send_task(void *p) {
  while (1) {
    gun_comm__send_data();
    vTaskDelay(30);
  }
}

#include "uart.h"
#include <stdlib.h>
#include <string.h>

#include "mp3.h"

// 'static' to make these functions 'private' to this file
static void create_blinky_tasks(void);
static void create_uart_task(void);
static void blink_task(void *params);
static void uart_task(void *params);

void send_mp3_task(void *p) {
  mp3__send_command(C_SEL_DEV, D_TF_CARD);
  while (1) {
    mp3__send_command(C_PLAY_W_VOL, 0x0201);
    vTaskDelay(4000);
  }
}

static void graphics_task(void *p);
static void display_task(void *p);

static void receive_zigbee_task(void *p);
static void gun_shot_task(void *p);

int main(void) {
  // create_blinky_tasks();
  // create_uart_task();
  // controller_data_update_mutex = xSemaphoreCreateMutex();

  // LED Matrix tasks
  // zigbee__comm_init(true);
  // mp3__init();
  // xTaskCreate(send_mp3_task, "uart", 2048 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);
  xTaskCreate(display_task, "display", 1024 / sizeof(void *), NULL, PRIORITY_HIGH, NULL);
  xTaskCreate(graphics_task, "graphics", 1024 / sizeof(void *), NULL, PRIORITY_LOW, NULL);
  xTaskCreate(receive_zigbee_task, "zigbee_receive", 2048 / sizeof(void *), NULL, PRIORITY_HIGH, NULL);
  xTaskCreate(gun_shot_task, "gun shot detected", 1024 / sizeof(void *), NULL, PRIORITY_LOW, NULL);

  // Joystick related tasks
  // zigbee__comm_init(false);
  // xTaskCreate(joystick_task, "read_joystick", 2048 / sizeof(void *), NULL, PRIORITY_LOW, NULL);

  // Gun related tasks
  // gun__init();
  // zigbee__comm_init(false);
  // xTaskCreate(gun_send_task, "send_gun_param", 2048 / sizeof(void *), NULL, PRIORITY_LOW, NULL);
  // xTaskCreate(gunFire_send_task, "send gun fire", 1024 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);

  puts("Starting RTOS");
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
    led_matrix__update_display();

    vTaskDelay(2);
  }
}

void gun_shot_task(void *p) {
  while (1) {
    if (xSemaphoreTake(gun_shot_detect_semaphore, portMAX_DELAY)) {
      detect_click(zigbee_gun_message[X_coord], zigbee_gun_message[Y_coord], 1);
    }
  }
}

void graphics_task(void *p) {
  graphics__turn_off_all_leds();
  initialize_object_details();
  // char test = "test";

  // void (*draw_enemy_pointer)(void);
  // draw_enemy_pointer = &draw_enemy;

  // uint8_t hit = 1;
  while (1) {
    // print_char(test, 9, 5, BLUE);

    // shape_update(10, 20, a1, GREEN, FRIEND);
    // shape_update(10, 20, a2, RED, FRIEND);

    // for (int i = 0; i < 8; i++)
    //   for (int j = 0; j < 8; j++) {
    //     shape_update(i * 8, j * 8, a1, i + 1, ENEMY);
    //     shape_update(i * 8, j * 8, a2, j + 1, ENEMY);
    //     shape_update(i * 8, j * 8, a3, (rand() % 8) + 1, ENEMY);
    //     shape_update(5, 10, cursor, WHITE, ENEMY);

    //     // shape_update(x, y, a3, BLUE, ENEMY);
    //   }

    // randomizer_objects();
    randomizer_objects_level_1();
    led_matrix__clear_data_buffer();
    shape_update(zigbee_joystick_message[X_coord], zigbee_joystick_message[Y_coord], a3, BLUE, FRIEND);
    // draw_enemy_pointer();

    draw_from_structure();
    // uint8_t hit;
    // if (zigbee_gun_message[Button_press] != 0)
    //   hit = 1;
    // else
    //   hit = 0;
    // printf("gun shot value is %d\n", hit);
    led_matrix__set_pixel(zigbee_gun_message[X_coord], 63 - zigbee_gun_message[Y_coord], RED);
    // detect_click(zigbee_gun_message[X_coord], zigbee_gun_message[Y_coord], zigbee_gun_message[Button_press]);
    //(32, 32, 1);

    // char test1[] = "son";
    // print_char(test1, 0, 32, RED);

    print_score(enemy_score, 1, 32, RED);
    print_score(life, 1, 0, GREEN);

    // detect_click(zigbee_joystick_message[X_coord], zigbee_joystick_message[Y_coord], hit);
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