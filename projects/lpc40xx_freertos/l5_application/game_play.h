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
 *
 */
game_play_level game_play__get_current_level(void);

/**
 * This function
 */
uint32_t game_play__level_manager(void);

/**
 *
 */
uint32_t game_play__graphics_manager(void);