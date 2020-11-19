#include "stdio.h"

uint64_t led_board_red[128];
uint64_t led_board_blue[128];
uint64_t led_board_green[128];

uint32_t game_board_red[25];
uint32_t game_board_blue[25];
uint32_t game_board_green[25];

uint32_t game_score_board[5];

uint32_t game_score_timer[5];

uint32_t game_next_pieces_red[6];
uint32_t game_next_pieces_blue[6];
uint32_t game_next_pieces_green[6];

uint32_t game_save_pieces_red[2];
uint32_t game_save_pieces_blue[2];
uint32_t game_save_pieces_green[2];

void led_display_init(void);
void led_display_clk(void);
void led_display_set_addr(uint8_t row);
void led_display_latch(uint8_t sig);
void led_display_draw_row();
void led_display_draw_frame(void);
void led_display_draw_single_pixel(uint8_t column, uint8_t row, uint8_t r, uint8_t g, uint8_t b);

void board_draw_single_pixel(uint8_t column, uint8_t row, uint8_t r, uint8_t g, uint8_t b);
void game_draw_display();
void game_draw_game_board(uint8_t display_score_param);
void game_title_change(uint32_t position, uint32_t display_type);
void game_board_score_draw(int number);
void game_board_timer(int seconds);
void game_board_next_piece(uint8_t piece, uint8_t position);
void game_board_save_piece(uint8_t piece);
