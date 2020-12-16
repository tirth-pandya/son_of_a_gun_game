#include "stdint.h"

typedef enum game_play_level {
  STARTUP,
  LEVEL_1_TRANSITION,
  LEVEL_1,
  LEVEL_2_TRANSITION,
  LEVEL_2,
  LEVEL_3_TRANSITION,
  LEVEL_3,
  LEVEL_4_TRANSITION,
  LEVEL_4,
  GAME_WINNER,
  GAME_OVER_LEVEL,
  MAX_GAME_LEVELS,
} game_play_level;

/**
 * This function manages game play current level and assigns the duration of the current game play level
 * @return The game play duration for the current level
 */
uint32_t game_play__level_manager(void);

/**
 * This function manages the graphics activities for the current game play level. e.g., it assigns the
 * number of enemies, the speed of the gameplay.
 */
uint32_t game_play__graphics_manager(void);

/**
 * It is responsible for namaging life object by calculating the chance of reviving life object based
 * on the given priority for each game play level.
 */
void game_play__life_object_manager(void);

void game_play__update_game_over_level(void);