#include "stdio.h"

#include "FreeRTOS.h"
#include "task.h"

#include "cli_handlers.h"
#include "clock.h"
#include "delay.h"
#include "event_groups.h"
#include "ff.h"
#include "gpio.h"
#include "led_display_driver.h"
#include "lpc40xx.h"
#include "sj2_cli.h"
#include "string.h"
#include "uart.h"
#include <stdlib.h>

TaskHandle_t producer_handle;
TaskHandle_t consumer_handle;
TaskHandle_t game_logic_handle;
TaskHandle_t game_over_handle;
TaskHandle_t title_color_handle;
TaskHandle_t game_player_logic_handle;
TaskHandle_t game_timer_handle;
TaskHandle_t game_menu_title;
TaskHandle_t game_menu_handle;

// piece_in_play current_piece;

static uint8_t saved_piece_id;
static uint8_t save_piece_state = 0;
bool save_was_pressed = false;
bool enable_piece_swap = false;
bool game_over_set = false;
static int difficulty_speed = 0;
static int last_score = 1337;

void clear_board_animation(void *p) {
  vTaskSuspend(NULL);
  while (1) {
    vTaskSuspend(game_logic_handle);
    for (int i = 0; i < 24; i++) {
      board_clear_game_board(i);
      vTaskDelay(75);
    }
    game_board_score_draw(0);
    vTaskResume(game_logic_handle);
    game_over_set = true;
    vTaskSuspend(title_color_handle);
    vTaskSuspend(NULL);
  }
}

void display(void *p) {
  while (1) {

    led_display_draw_frame();
    led_display_draw_single_pixel(30, 62, 1, 1, 1);
    vTaskDelay(15);
  }
}

int main(void) {
  led_display_init();
  game_draw_display();
  uint32_t pclk = clock__get_peripheral_clock_hz;
  uart__init(UART__3, pclk, 115200);
  gpio__construct_with_function(GPIO__PORT_4, 28, GPIO__FUNCTION_2); // TX

  // game_draw_display();
  // game_draw_game_board(0);
  // game_board_timer(0);

  xTaskCreate(display, "display", 1024U / sizeof(void *), NULL, PRIORITY_LOW, NULL);

  sj2_cli__init();

  puts("Starting RTOS");
  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}