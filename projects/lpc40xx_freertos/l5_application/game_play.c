#include "game_play.h"
#include "FreeRTOS.h"
#include "alphabets.h"
#include "led_matrix.h"
#include "mp3.h"
#include "object_tracking.h"
#include "stdlib.h"
#include "task.h"
#include <stdbool.h>

extern uint8_t life, enemy_score;

static game_play_level current_level = STARTUP;
static game_play_level next_level = STARTUP;
static uint8_t max_game_score = 00;

static void update_max_score(uint16_t current_score) {
  max_game_score = (max_game_score < current_score) ? current_score : max_game_score;
}

uint32_t game_play__level_manager(void) {
  uint32_t game_level_pause = 0;
  current_level = next_level;
  switch (current_level) {
  case STARTUP:
    game_level_pause = 15 * 1000;
    next_level = LEVEL_1_TRANSITION;
    break;

  case LEVEL_1_TRANSITION:
    game_level_pause = 1 * 1000;
    next_level = LEVEL_1;
    update_mp3_details(LEVEL_UP, levelup_duration);
    break;

  case LEVEL_1:
    game_level_pause = 10 * 1000;
    next_level = LEVEL_2_TRANSITION;

    break;

  case LEVEL_2_TRANSITION:
    game_level_pause = 1 * 1000;
    next_level = LEVEL_2;
    update_mp3_details(LEVEL_UP, levelup_duration);
    break;

  case LEVEL_2:
    game_level_pause = 10 * 1000;
    next_level = LEVEL_3_TRANSITION;
    break;

  case LEVEL_3_TRANSITION:
    game_level_pause = 1 * 1000;
    next_level = LEVEL_3;
    update_mp3_details(LEVEL_UP, levelup_duration);
    break;

  case LEVEL_3:
    game_level_pause = 20 * 1000;
    next_level = GAME_WINNER;
    break;

  case LEVEL_4_TRANSITION:
    game_level_pause = 10 * 1000;
    next_level = LEVEL_4;
    update_mp3_details(LEVEL_UP, levelup_duration);
    break;

  case LEVEL_4:
    game_level_pause = 20 * 1000;
    next_level = GAME_WINNER;
    break;

  case GAME_WINNER:
    game_level_pause = 3 * 1000;
    next_level = STARTUP;
    break;

  case GAME_OVER_LEVEL:
    game_level_pause = 3 * 1000;
    next_level = STARTUP;
    update_mp3_details(GAME_OVER, gameover_duration);
    break;

  default:
    break;
  }
  return game_level_pause;
}

uint32_t game_play__graphics_manager(void) {

  static uint8_t number_of_live_enemies;
  static uint16_t game_play_speed = 50;
  bool transition = false;
  uint8_t row = 28;
  uint8_t col = 15;
  switch (current_level) {
  case STARTUP:
    initialize_object_details();
    transition = true;
    led_matrix__clear_data_buffer();
    draw_welcome(5, 9);
    print_score(max_game_score, 50, 31, RED);
    break;

  case LEVEL_1_TRANSITION:
    led_matrix__clear_data_buffer();
    transition = true;
    char level1_string[6] = "level";
    print_char(level1_string, row, col, RED);
    char level1_number[3] = " 1";
    print_char(level1_number, row, col + 25, RED);
    break;

  case LEVEL_1:
    transition = false;
    game_play_speed = 100;
    number_of_live_enemies = 5;
    randomizer_objects_level_1();
    break;

  case LEVEL_2_TRANSITION:
    transition = true;
    led_matrix__clear_data_buffer();
    char level2_string[6] = "level";
    print_char(level2_string, row, col, RED);
    char level2_number[3] = " 2";
    print_char(level2_number, row, col + 25, RED);
    break;

  case LEVEL_2:
    transition = false;
    game_play_speed = 70;
    number_of_live_enemies = 8;
    randomizer_objects_level_2();
    break;

  case LEVEL_3_TRANSITION:
    transition = true;
    led_matrix__clear_data_buffer();
    char level3_string[6] = "level";
    print_char(level3_string, row, col, RED);
    char level3_number[3] = " 3";
    print_char(level3_number, row, col + 25, RED);
    break;

  case LEVEL_3:
    transition = false;
    number_of_live_enemies = 10;
    game_play_speed = 50;
    randomizer_objects_level_3();
    break;

  case GAME_WINNER:
    transition = true;
    led_matrix__clear_data_buffer();
    update_max_score(enemy_score);
    break;

  case GAME_OVER_LEVEL:
    transition = true;
    char level_over_string[10] = "game over";
    led_matrix__clear_data_buffer();
    print_char(level_over_string, row, 2, RED);
    update_max_score(enemy_score);
    break;

  default:
    break;
  }
  printf("Current level is %d\n", current_level);

  if (!transition) {
    update_required_enemies_status(number_of_live_enemies);
    // Clear previous data

    led_matrix__clear_data_buffer();
    // update_alive_enemies(number_of_live_enemies);
    update_friend_location();
    draw_road();

    // Draw all the live objects
    draw_from_structure();

    // Detect the collision
    collision_detection();
    collision_detection_for_life();

    print_score(max_game_score, 58, 46, RED);
    draw_crown(58, 36);

    // Update the score
    print_score(enemy_score, 1, 40, RED);
    print_score(life, 1, 8, GREEN);
  }
  return game_play_speed;
}

void game_play__update_game_over_level(void) {
  current_level = GAME_OVER_LEVEL;
  // next_level = STARTUP;
}

void game_play__life_object_manager(void) {
  uint8_t life_object_probability = 0;
  switch (current_level) {
  case LEVEL_1:
    life_object_probability = rand() % 5;
    if (life_object_probability == 4)
      object_tracking__revive_life_object();
    break;

  case LEVEL_2:
    life_object_probability = rand() % 3;
    if (life_object_probability == 2)
      object_tracking__revive_life_object();
    break;

  case LEVEL_3:
    life_object_probability = rand() % 2;
    if (life_object_probability == 1)
      object_tracking__revive_life_object();
    break;

  default:
    break;
  }
}