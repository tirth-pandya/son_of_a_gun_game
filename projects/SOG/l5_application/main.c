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

#include "mp3.h"

#include "uart.h"
#include <stdlib.h>
#include <string.h>

#include "game_play.h"

extern volatile uint8_t zigbee_joystick_message[Max_message_elemets];
extern volatile uint8_t zigbee_gun_message[Max_message_elemets];

uint8_t change_song = 1;
#ifdef DEF_TASK
// 'static' to make these functions 'private' to this file
static void create_blinky_tasks(void);
static void create_uart_task(void);
static void blink_task(void *params);
static void uart_task(void *params);
#endif

static void graphics_task(void *p);
static void display_task(void *p);
static void receive_zigbee_task(void *p);
static void gun_shot_detect_task(void *p);
static void joystick_task(void *p);
static void gun_send_task(void *p);
static void send_mp3_task(void *p);
static void controller_object_display_task(void *p);
static void game_play_level_monitor_task(void *p);
static void graphics_life_object_manager_task(void *p);

int main(void) {
  srand(time(0));

  // LED Matrix tasks
  zigbee__comm_init(true);
  mp3__init();
  xTaskCreate(send_mp3_task, "uart", 2048 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);
  xTaskCreate(display_task, "display", 1024 / sizeof(void *), NULL, PRIORITY_HIGH, NULL);
  xTaskCreate(graphics_task, "graphics", 1024 / sizeof(void *), NULL, PRIORITY_LOW, NULL);
  xTaskCreate(receive_zigbee_task, "zigbee_receive", 2048 / sizeof(void *), NULL, PRIORITY_HIGH, NULL);
  xTaskCreate(gun_shot_detect_task, "gun shot detected", 1024 / sizeof(void *), NULL, PRIORITY_LOW, NULL);
  xTaskCreate(game_play_level_monitor_task, "Update_game_level", 4096 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);
  xTaskCreate(controller_object_display_task, "gun pointer, friend object display", 4096 / sizeof(void *), NULL,
              PRIORITY_MEDIUM, NULL);
  xTaskCreate(graphics_life_object_manager_task, "manage life object", 1024 / sizeof(void *), NULL, PRIORITY_MEDIUM,
              NULL);

  // Joystick related tasks
  // zigbee__comm_init(false);
  // xTaskCreate(joystick_task, "read_joystick", 2048 / sizeof(void *), NULL, PRIORITY_LOW, NULL);

  // Gun related tasks
  // gun__init();
  // zigbee__comm_init(false);
  // xTaskCreate(gun_send_task, "send_gun_param", 2048 / sizeof(void *), NULL, PRIORITY_LOW, NULL);

  puts("Starting RTOS");
  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}

//**************************************************************************************************
// LED Matrix tasks **********************************************************************************
//**************************************************************************************************
void display_task(void *p) {
  led_matrix_init();

  while (1) {
    led_matrix__update_display();

    vTaskDelay(2);
  }
}

void controller_object_display_task(void *p) {
  while (1) {
    shape_update(zigbee_gun_message[X_coord], zigbee_gun_message[Y_coord], cursor, WHITE, NONE);
    // led_matrix__set_pixel(zigbee_gun_message[X_coord], 63 - zigbee_gun_message[Y_coord], RED);
    // shape_update(zigbee_joystick_message[X_coord], zigbee_joystick_message[Y_coord], enemy_3, BLUE, FRIEND);
    // draw_friend(zigbee_joystick_message[X_coord], zigbee_joystick_message[Y_coord]);
    vTaskDelay(50);
  }
}

void game_play_level_monitor_task(void *p) {
  static uint32_t game_level_pause;
  while (1) {
    game_level_pause = game_play__level_manager();

    printf("Current level pause is %ld\n", game_level_pause);
    vTaskDelay(game_level_pause);
  }
}

void graphics_task(void *p) {
  graphics__turn_off_all_leds();

  static uint8_t number_of_live_enemies;

  static uint16_t game_play_speed = 100;
  while (1) {

    game_play_speed = game_play__graphics_manager();

    vTaskDelay(game_play_speed);
  }
}

void graphics_life_object_manager_task(void *p) {
  while (1) {
    game_play__life_object_manager();
    vTaskDelay(10 * 1000);
  }
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

void send_mp3_task(void *p) {
  mp3__send_command(C_SEL_DEV, D_TF_CARD);
  change_song = 1;
  bool play_once = true;
  update_mp3_details(DEFAULT_BG, default_bg_duration);
  while (1) {
    switch (mp3_details.mp3_to_play) {
    case DEFAULT_BG:
      if (change_song == 1) {
        change_song = 0;
        mp3__send_command(C_ONE_CY_PLAY_FOLD, 0x0101);
        fprintf(stderr, "Playing bg\n");
      }
      vTaskDelay(5);
      break;

    case GUNSHOT:
      mp3__send_command(C_PLAY_FOLD_FILE, 0x0201);
      vTaskDelay(mp3_details.mp3_duration);
      change_song = 1;
      update_mp3_details(DEFAULT_BG, default_bg_duration - 100);
      break;

    case LEVEL_UP:
      play_once = true;
      mp3__send_command(C_PLAY_FOLD_FILE, 0x0501);
      vTaskDelay(mp3_details.mp3_duration);
      change_song = 1;
      update_mp3_details(DEFAULT_BG, default_bg_duration - 100);
      break;

    case GAME_OVER:
      if (play_once) {
        mp3__send_command(C_PLAY_FOLD_FILE, 0x0601);
        play_once = false;
      }

      vTaskDelay(mp3_details.mp3_duration);
      change_song = 1;
      update_mp3_details(DEFAULT_BG, default_bg_duration - 100);

      break;
    default:
      vTaskDelay(400);
      break;
    }
  }
}

void gun_shot_detect_task(void *p) {
  while (1) {
    if (xSemaphoreTake(gun_shot_detect_semaphore, portMAX_DELAY)) {
      // This is offset for cursor adjustment.
      detect_click(zigbee_gun_message[X_coord] + 1, zigbee_gun_message[Y_coord] + 2, 1);
    }
  }
}

//**************************************************************************************************
// Joystick tasks **********************************************************************************
//**************************************************************************************************
void joystick_task(void *p) {
  gpio_s x = {0, 25};
  gpio_s y = {1, 30};
  gpio_s s_k = {1, 31};
  joystick__initialize(x, y, s_k);
  zigbee__cs();
  while (1) {
    joystick_comm__send();
    vTaskDelay(20);
  }
}

//**************************************************************************************************
// Gun tasks **********************************************************************************
//**************************************************************************************************

void gun_send_task(void *p) {
  while (1) {
    gun_comm__send_data();
    vTaskDelay(30);
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