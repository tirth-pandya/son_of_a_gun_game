#pragma once

#include <stdbool.h>
#include <stdint.h>
/*
MP3_DECODER  |  SJTWO
_____________|_____________
    Vcc      |  3.3V
    Tx       |  Rx - P4.29
    Rx       |  Tx - P4.28
    GND      |  GND
_____________|_____________
*/

#define C_NEXT_SONG 0x01
#define C_PREV_SONG 0x02
#define C_INDEX_SONG 0x03
#define C_VOL_UP 0X04
#define C_VOL_DOWN 0x05
#define C_VOL_SET 0x06
#define C_1_CY_PLAY_1ST_FILE 0x08
#define C_SEL_DEV 0x09
#define C_SLEEP 0x0a
#define C_WAKE_UP 0x0b
#define C_RESET 0x0c
#define C_PLAY 0x0d
#define C_PAUSE 0x0e
#define C_PLAY_FOLD_FILE 0x0f
#define C_STOP 0x16
#define C_ONE_CY_PLAY_FOLD 0x17
#define C_ONE_CY_PLAY_FILE 0x19
#define C_SET_DAC 0x1a
#define C_PLAY_W_VOL 0x22

#define D_TF_CARD 0x02

#define C_D_NONE 0x00

typedef enum { DEFAULT_BG = 0, GUNSHOT, GAME_OVER, NONE } MP3_SOUNDS;
const uint32_t default_bg_duration, gunshot_duration;
typedef struct mp3_details_s {
  MP3_SOUNDS mp3_to_play;
  uint32_t mp3_duration;
};
extern struct mp3_details_s mp3_details;

bool mp3__init();
bool mp3__send_command(uint8_t command, uint16_t data);
void update_mp3_details(MP3_SOUNDS, uint32_t);
