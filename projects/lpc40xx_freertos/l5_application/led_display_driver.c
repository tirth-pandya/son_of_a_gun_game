#include "led_display_driver.h"
#include "delay.h"
#include "gpio.h"
#include "lpc40xx.h"
#include "pwm1.h"

gpio_s u_red;
gpio_s u_blue;
gpio_s u_green;

gpio_s l_red;
gpio_s l_blue;
gpio_s l_green;

gpio_s latch;
gpio_s clk;

gpio_s addr_a;
gpio_s addr_b;
gpio_s addr_c;
gpio_s addr_d;
gpio_s addr_e;

gpio_s oe;

void led_display_init() {
  addr_a = gpio__construct_as_output(GPIO__PORT_1, 20);
  addr_b = gpio__construct_as_output(GPIO__PORT_1, 23);
  addr_c = gpio__construct_as_output(GPIO__PORT_1, 28);
  addr_d = gpio__construct_as_output(GPIO__PORT_1, 29);
  addr_e = gpio__construct_as_output(GPIO__PORT_1, 30);

  oe = gpio__construct_as_output(GPIO__PORT_2, 2);
  clk = gpio__construct_as_output(GPIO__PORT_2, 0);
  latch = gpio__construct_as_output(GPIO__PORT_2, 1);
  u_red = gpio__construct_as_output(GPIO__PORT_0, 6);
  u_green = gpio__construct_as_output(GPIO__PORT_0, 7);
  u_blue = gpio__construct_as_output(GPIO__PORT_0, 8);

  l_red = gpio__construct_as_output(GPIO__PORT_0, 26);
  l_green = gpio__construct_as_output(GPIO__PORT_0, 25);
  l_blue = gpio__construct_as_output(GPIO__PORT_1, 31);

  gpio__reset(addr_a);
}

void led_display_clk() {
  gpio__set(clk);
  gpio__reset(clk);
}

void led_display_latch(uint8_t sig) {
  if (sig == 0) {
    gpio__reset(latch);
    gpio__reset(oe);
    delay__us(1);
  } else {
    gpio__set(latch);
    gpio__set(oe);
    delay__us(1);
  }
}

void led_display_set_addr(uint8_t row) {

  if (row & 1) {
    gpio__reset(addr_a);
  } else {
    gpio__set(addr_a);
  }
  if ((row & (1 << 1))) {
    gpio__reset(addr_b);
  } else {
    gpio__set(addr_b);
  }
  if ((row & (1 << 2))) {
    gpio__reset(addr_c);
  } else {
    gpio__set(addr_c);
  }
  if ((row & (1 << 3))) {
    gpio__reset(addr_d);
  } else {
    gpio__set(addr_d);
  }
  /*
  if ((row & (1 << 4))) {
    gpio__reset(addr_e);
  } else {
    gpio__set(addr_e);
  }*/
}

void led_display_draw_frame() {

  int a = 0, offset = 32;

  for (int i = 0; i < 32; i++) {

    // led_display_set_addr(i - 1);
    led_display_set_addr(i);
    gpio__reset(latch);
    gpio__reset(oe);

    for (a = 64; a >= 0; a--) {

      if (led_board_red[i] & (1 << a))
        gpio__set(l_red);
      else
        gpio__reset(l_red);

      if (led_board_red[i + offset] & (1 << a))
        gpio__set(u_red);
      else
        gpio__reset(u_red);

      if (led_board_green[i + offset] & (1 << a))
        gpio__set(l_green);
      else
        gpio__reset(l_green);

      if (led_board_green[i] & (1 << a))
        gpio__set(u_green);
      else
        gpio__reset(u_green);

      if (led_board_blue[i] & (1 << a))
        gpio__set(l_blue);
      else
        gpio__reset(l_blue);

      if (led_board_blue[i + offset] & (1 << a))
        gpio__set(u_blue);
      else
        gpio__reset(u_blue);
      gpio__set(latch);
      gpio__reset(latch);
      led_display_clk();
    }
    gpio__set(latch);
    gpio__set(oe);
  }
  delay__us(250);
}

void game_draw_display() {

  for (int i = 0; i < 64; i++) {
    led_board_blue[i] = 0;
    led_board_red[i] = 0;
    led_board_green[i] = 0;
  }
  led_board_green[12] = 0b10000000000000000000000000000001;
  // led_board_green[60] = 0b1010100000000000000000000001101111111111111111111111111111111111;
  led_board_green[60] = 0xF000000FF000000F;
  /* led_board_red[63] = 0b11111110000011111111111110000000;
   led_board_red[0] = 0b11111111111110000001111100000000;

   led_board_green[52] = 0b11001111110000000000111110001111;
   led_board_green[22] = 0b11001111100111100000000001111000;
   led_board_green[21] = 0b11111111111111111111110000000000;

   led_board_blue[55] = 0b10101000000000000000000000011011;
   led_board_blue[42] = 0b11111000000000000000000000000000;*/
  // led_board_blue[12] = 0b10000000000000011000000000000001;
}

void led_display_draw_single_pixel(uint8_t column, uint8_t row, uint8_t r, uint8_t g, uint8_t b) {

  if (r > 0)
    led_board_red[row] |= (1 << column);
  else
    led_board_red[row] &= ~(1 << column);

  if (g > 0)
    led_board_green[row] |= (1 << column);
  else
    led_board_green[row] &= ~(1 << column);

  if (b > 0)
    led_board_blue[row] |= (1 << column);
  else
    led_board_blue[row] &= ~(1 << column);
}

void board_draw_single_pixel(uint8_t column, uint8_t row, uint8_t r, uint8_t g, uint8_t b) {

  if (row > 23 || column > 10) // Don't draw something out of bounds
    return;

  if (r > 0)
    game_board_red[row] |= (1 << column);
  else
    game_board_red[row] &= ~(1 << column);

  if (g > 0)
    game_board_green[row] |= (1 << column);
  else
    game_board_green[row] &= ~(1 << column);

  if (b > 0)
    game_board_blue[row] |= (1 << column);
  else
    game_board_blue[row] &= ~(1 << column);
}

void game_draw_game_board(uint8_t display_score_param) {

  static uint8_t display_score = 0;
  if (display_score_param == 1)
    display_score = display_score_param;
  else if (display_score_param == 2)
    display_score = 0;

  for (int i = 0; i < 25; i++) {
    if (i != 0) {
      led_board_red[i] &= ~(0x3FF << 21);
      led_board_red[i] |= (game_board_red[i - 1] << 21);
      led_board_blue[i] &= ~(0x3FF << 21);
      led_board_blue[i] |= (game_board_blue[i - 1] << 21);
      led_board_green[i] &= ~(0x3FF << 21);
      led_board_green[i] |= (game_board_green[i - 1] << 21);
    }
    if (display_score) {
      if (i < 5) {
        led_board_blue[i + 27] &= ~(0xFFFFFF);
        led_board_blue[i + 27] |= (game_score_board[i]);
      }
    }
    if (i == 1) {
      led_board_blue[i + 4] &= ~(0x1FFFF << 1);
      led_board_blue[i + 3] &= ~(0x1FFFF << 1);
      led_board_blue[i + 2] &= ~(0x1FFFF << 1);
      led_board_blue[i + 1] &= ~(0x1FFFF << 1);
      led_board_blue[i] &= ~(0x1FFFF << 1);

      led_board_blue[i + 4] |= (game_score_timer[4] << 1);
      led_board_blue[i + 3] |= (game_score_timer[3] << 1);
      led_board_blue[i + 2] |= (game_score_timer[2] << 1);
      led_board_blue[i + 1] |= (game_score_timer[1] << 1);
      led_board_blue[i] |= (game_score_timer[0] << 1);
    }
    if (i == 22) {
      led_board_blue[i] &= ~(0xF0F << 6);
      led_board_blue[i + 1] &= ~(0xF0F << 6);
      led_board_red[i] &= ~(0xF0F << 6);
      led_board_red[i + 1] &= ~(0xF0F << 6);
      led_board_green[i] &= ~(0xF0F << 6);
      led_board_green[i + 1] &= ~(0xF0F << 6);
      led_board_blue[i] |= (game_next_pieces_blue[0] << 14);
      led_board_blue[i + 1] |= (game_next_pieces_blue[1] << 14);
      led_board_green[i] |= (game_next_pieces_green[0] << 14);
      led_board_green[i + 1] |= (game_next_pieces_green[1] << 14);
      led_board_red[i] |= (game_next_pieces_red[0] << 14);
      led_board_red[i + 1] |= (game_next_pieces_red[1] << 14);

      led_board_blue[i + 1 - 6] &= ~(0xF << 14);
      led_board_blue[i + 2 - 6] &= ~(0xF << 14);
      led_board_red[i + 1 - 6] &= ~(0xF << 14);
      led_board_red[i + 2 - 6] &= ~(0xF << 14);
      led_board_green[i + 1 - 6] &= ~(0xF << 14);
      led_board_green[i + 2 - 6] &= ~(0xF << 14);
      led_board_blue[i + 1 - 6] |= (game_next_pieces_blue[2] << 14);
      led_board_blue[i + 2 - 6] |= (game_next_pieces_blue[3] << 14);
      led_board_green[i + 1 - 6] |= (game_next_pieces_green[2] << 14);
      led_board_green[i + 2 - 6] |= (game_next_pieces_green[3] << 14);
      led_board_red[i + 1 - 6] |= (game_next_pieces_red[2] << 14);
      led_board_red[i + 2 - 6] |= (game_next_pieces_red[3] << 14);

      led_board_blue[i + 1 - 11] &= ~(0xF << 14);
      led_board_blue[i + 2 - 11] &= ~(0xF << 14);
      led_board_red[i + 1 - 11] &= ~(0xF << 14);
      led_board_red[i + 2 - 11] &= ~(0xF << 14);
      led_board_green[i + 1 - 11] &= ~(0xF << 14);
      led_board_green[i + 2 - 11] &= ~(0xF << 14);
      led_board_blue[i + 1 - 11] |= (game_next_pieces_blue[4] << 14);
      led_board_blue[i + 2 - 11] |= (game_next_pieces_blue[5] << 14);
      led_board_green[i + 1 - 11] |= (game_next_pieces_green[4] << 14);
      led_board_green[i + 2 - 11] |= (game_next_pieces_green[5] << 14);
      led_board_red[i + 1 - 11] |= (game_next_pieces_red[4] << 14);
      led_board_red[i + 2 - 11] |= (game_next_pieces_red[5] << 14);

      led_board_blue[i] |= (game_save_pieces_blue[0] << 6);
      led_board_blue[i + 1] |= (game_save_pieces_blue[1] << 6);
      led_board_green[i] |= (game_save_pieces_green[0] << 6);
      led_board_green[i + 1] |= (game_save_pieces_green[1] << 6);
      led_board_red[i] |= (game_save_pieces_red[0] << 6);
      led_board_red[i + 1] |= (game_save_pieces_red[1] << 6);
    }
  }
}

void game_title_change(uint32_t position, uint32_t display_type) {
  uint32_t r31 = 0;
  uint32_t r30 = 0;
  uint32_t r29 = 0;
  uint32_t r28 = 0;
  uint32_t r27 = 0;

  if (display_type == 1) {
    r31 = game_score_board[4];
    r30 = game_score_board[3];
    r29 = game_score_board[2];
    r28 = game_score_board[1];
    r27 = game_score_board[0];
    if (position == 0)
      game_draw_game_board(1);
  } else {
    r31 = 0b1111101110111110111001110011110;
    r30 = 0b0010001000001000100100100100000;
    r29 = 0b0010001100001000111000100011100;
    r28 = 0b0010001000001000101000100000010;
    r27 = 0b0010001110001000100101110111100;
    game_draw_game_board(2);
  }
  led_board_blue[31] &= ~(7 << position);
  led_board_blue[31] |= r31 & (7 << position);
  led_board_green[31] = 0;
  led_board_green[31] |= r31 & (7 << position);

  led_board_blue[30] &= ~(7 << position);
  led_board_blue[30] |= r30 & (7 << position);
  led_board_green[30] = 0;
  led_board_green[30] |= r30 & (7 << position);

  led_board_blue[29] &= ~(7 << position);
  led_board_blue[29] |= r29 & (7 << position);
  led_board_green[29] = 0;
  led_board_green[29] |= r29 & (7 << position);

  led_board_blue[28] &= ~(7 << position);
  led_board_blue[28] |= r28 & (7 << position);
  led_board_green[28] = 0;
  led_board_green[28] |= r28 & (7 << position);

  led_board_blue[27] &= ~(7 << position);
  led_board_blue[27] |= r27 & (7 << position);
  led_board_green[27] = 0;
  led_board_green[27] |= r27 & (7 << position);
}

void game_board_score_draw(int number) {

  game_score_board[4] = 0;
  game_score_board[3] = 0;
  game_score_board[2] = 0;
  game_score_board[1] = 0;
  game_score_board[0] = 0;

  int temp = 0;

  if (number > 999999)
    number = 999999;
  {
    // First digit
    for (int i = 0; i < 8; i++) {
      if (i == 0) {
        temp = number % (10);
      } else if (i == 1) {
        if (number > 9)
          temp = (number % 100) / (10);
        else
          temp = 0;
      } else if (i == 2) {
        if (number > 99)
          temp = (number % 1000) / (100);
        else
          temp = 0;
      } else if (i == 3) {
        if (number > 999)
          temp = (number % 10000) / (1000);
        else
          temp = 0;
      } else if (i == 4) {
        if (number > 9999)
          temp = (number % 100000) / (10000);
        else
          temp = 0;
      } else if (i == 5) {
        if (number > 99999)
          temp = (number % 1000000) / (100000);
        else
          temp = 0;
      } else if (i == 6) {
        if (number > 999999)
          temp = (number % 10000000) / (1000000);
        else
          temp = 0;
      }

      if (temp == 0) {
        game_score_board[4] |= (0b111 << ((4 * i)));
        game_score_board[3] |= (0b101 << ((4 * i)));
        game_score_board[2] |= (0b101 << ((4 * i)));
        game_score_board[1] |= (0b101 << ((4 * i)));
        game_score_board[0] |= (0b111 << ((4 * i)));
      } else if (temp == 1) {
        game_score_board[4] |= (0b001 << ((4 * i)));
        game_score_board[3] |= (0b001 << ((4 * i)));
        game_score_board[2] |= (0b001 << ((4 * i)));
        game_score_board[1] |= (0b001 << ((4 * i)));
        game_score_board[0] |= (0b001 << ((4 * i)));
      } else if (temp == 2) {
        game_score_board[4] |= (0b111 << ((4 * i)));
        game_score_board[3] |= (0b001 << ((4 * i)));
        game_score_board[2] |= (0b111 << ((4 * i)));
        game_score_board[1] |= (0b100 << ((4 * i)));
        game_score_board[0] |= (0b111 << ((4 * i)));
      } else if (temp == 3) {
        game_score_board[4] |= (0b111 << ((4 * i)));
        game_score_board[3] |= (0b001 << ((4 * i)));
        game_score_board[2] |= (0b111 << ((4 * i)));
        game_score_board[1] |= (0b001 << ((4 * i)));
        game_score_board[0] |= (0b111 << ((4 * i)));
      } else if (temp == 4) {
        game_score_board[4] |= (0b101 << ((4 * i)));
        game_score_board[3] |= (0b101 << ((4 * i)));
        game_score_board[2] |= (0b111 << ((4 * i)));
        game_score_board[1] |= (0b001 << ((4 * i)));
        game_score_board[0] |= (0b001 << ((4 * i)));
      } else if (temp == 5) {
        game_score_board[4] |= (0b111 << ((4 * i)));
        game_score_board[3] |= (0b100 << ((4 * i)));
        game_score_board[2] |= (0b110 << ((4 * i)));
        game_score_board[1] |= (0b001 << ((4 * i)));
        game_score_board[0] |= (0b110 << ((4 * i)));
      } else if (temp == 6) {
        game_score_board[4] |= (0b111 << ((4 * i)));
        game_score_board[3] |= (0b100 << ((4 * i)));
        game_score_board[2] |= (0b111 << ((4 * i)));
        game_score_board[1] |= (0b101 << ((4 * i)));
        game_score_board[0] |= (0b111 << ((4 * i)));
      } else if (temp == 7) {
        game_score_board[4] |= (0b111 << ((4 * i)));
        game_score_board[3] |= (0b001 << ((4 * i)));
        game_score_board[2] |= (0b001 << ((4 * i)));
        game_score_board[1] |= (0b001 << ((4 * i)));
        game_score_board[0] |= (0b001 << ((4 * i)));
      } else if (temp == 8) {
        game_score_board[4] |= (0b111 << ((4 * i)));
        game_score_board[3] |= (0b101 << ((4 * i)));
        game_score_board[2] |= (0b111 << ((4 * i)));
        game_score_board[1] |= (0b101 << ((4 * i)));
        game_score_board[0] |= (0b111 << ((4 * i)));
      } else if (temp == 9) {
        game_score_board[4] |= (0b111 << ((4 * i)));
        game_score_board[3] |= (0b101 << ((4 * i)));
        game_score_board[2] |= (0b111 << ((4 * i)));
        game_score_board[1] |= (0b001 << ((4 * i)));
        game_score_board[0] |= (0b001 << ((4 * i)));
      }
    }
  }
}

void game_board_next_piece(uint8_t piece, uint8_t position) {

  if (piece == 1) {
    game_next_pieces_blue[1 + (2 * position)] = 0b0100;
    game_next_pieces_blue[0 + (2 * position)] = 0b0111;
    game_next_pieces_red[1 + (2 * position)] = 0;
    game_next_pieces_red[0 + (2 * position)] = 0;
    game_next_pieces_green[1 + (2 * position)] = 0;
    game_next_pieces_green[0 + (2 * position)] = 0;
  } else if (piece == 2) {
    game_next_pieces_blue[1 + (2 * position)] = 0;
    game_next_pieces_blue[0 + (2 * position)] = 0;
    game_next_pieces_red[1 + (2 * position)] = 0;
    game_next_pieces_red[0 + (2 * position)] = 0;
    game_next_pieces_green[1 + (2 * position)] = 0b0011;
    game_next_pieces_green[0 + (2 * position)] = 0b0110;
  } else if (piece == 3) {
    game_next_pieces_blue[1 + (2 * position)] = 0;
    game_next_pieces_blue[0 + (2 * position)] = 0xF;
    game_next_pieces_red[1 + (2 * position)] = 0;
    game_next_pieces_red[0 + (2 * position)] = 0;
    game_next_pieces_green[1 + (2 * position)] = 0;
    game_next_pieces_green[0 + (2 * position)] = 0xF;
  } else if (piece == 4) {
    game_next_pieces_blue[1 + (2 * position)] = 0;
    game_next_pieces_blue[0 + (2 * position)] = 0;
    game_next_pieces_red[1 + (2 * position)] = 0b0110;
    game_next_pieces_red[0 + (2 * position)] = 0b0011;
    game_next_pieces_green[1 + (2 * position)] = 0;
    game_next_pieces_green[0 + (2 * position)] = 0;
  } else if (piece == 5) {
    game_next_pieces_blue[1 + (2 * position)] = 0b0010;
    game_next_pieces_blue[0 + (2 * position)] = 0b0111;
    game_next_pieces_red[1 + (2 * position)] = 0b0010;
    game_next_pieces_red[0 + (2 * position)] = 0b0111;
    game_next_pieces_green[1 + (2 * position)] = 0;
    game_next_pieces_green[0 + (2 * position)] = 0;
  } else if (piece == 6) {
    game_next_pieces_blue[1 + (2 * position)] = 0b0000;
    game_next_pieces_blue[0 + (2 * position)] = 0b0000;
    game_next_pieces_red[1 + (2 * position)] = 0b0110;
    game_next_pieces_red[0 + (2 * position)] = 0b0110;
    game_next_pieces_green[1 + (2 * position)] = 0b0110;
    game_next_pieces_green[0 + (2 * position)] = 0b0110;
  } else if (piece == 7) {
    game_next_pieces_blue[1 + (2 * position)] = 0b0001;
    game_next_pieces_blue[0 + (2 * position)] = 0b0111;
    game_next_pieces_red[1 + (2 * position)] = 0b0001;
    game_next_pieces_red[0 + (2 * position)] = 0b0111;
    game_next_pieces_green[1 + (2 * position)] = 0b0001;
    game_next_pieces_green[0 + (2 * position)] = 0b0111;
  }
}

void game_board_timer(int seconds) {

  game_score_timer[4] = 0;
  game_score_timer[3] = 0;
  game_score_timer[2] = 0;
  game_score_timer[1] = 0;
  game_score_timer[0] = 0;

  int temp = 0;
  int calculated_digit = 0;

  int digit_positions[4] = {0};

  // Timer - 00 : 00
  // [3][2] : [1][0]
  calculated_digit = seconds;
  calculated_digit = calculated_digit % 60;
  calculated_digit %= 10;
  digit_positions[0] = calculated_digit;

  calculated_digit = seconds;
  calculated_digit = ((calculated_digit % 60) / 10) % 10;
  digit_positions[1] = calculated_digit;

  calculated_digit = seconds;
  calculated_digit = (calculated_digit / 60) % 10;
  digit_positions[2] = calculated_digit;

  calculated_digit = seconds;
  calculated_digit = ((calculated_digit / 60) / 10) % 10;
  digit_positions[3] = calculated_digit;

  for (int i = 0; i < 4; i++) {

    temp = digit_positions[i];

    if (temp == 0) {
      game_score_timer[4] |= (0b111 << ((4 * i)));
      game_score_timer[3] |= (0b101 << ((4 * i)));
      game_score_timer[2] |= (0b101 << ((4 * i)));
      game_score_timer[1] |= (0b101 << ((4 * i)));
      game_score_timer[0] |= (0b111 << ((4 * i)));
    } else if (temp == 1) {
      game_score_timer[4] |= (0b001 << ((4 * i)));
      game_score_timer[3] |= (0b001 << ((4 * i)));
      game_score_timer[2] |= (0b001 << ((4 * i)));
      game_score_timer[1] |= (0b001 << ((4 * i)));
      game_score_timer[0] |= (0b001 << ((4 * i)));
    } else if (temp == 2) {
      game_score_timer[4] |= (0b111 << ((4 * i)));
      game_score_timer[3] |= (0b001 << ((4 * i)));
      game_score_timer[2] |= (0b111 << ((4 * i)));
      game_score_timer[1] |= (0b100 << ((4 * i)));
      game_score_timer[0] |= (0b111 << ((4 * i)));
    } else if (temp == 3) {
      game_score_timer[4] |= (0b111 << ((4 * i)));
      game_score_timer[3] |= (0b001 << ((4 * i)));
      game_score_timer[2] |= (0b111 << ((4 * i)));
      game_score_timer[1] |= (0b001 << ((4 * i)));
      game_score_timer[0] |= (0b111 << ((4 * i)));
    } else if (temp == 4) {
      game_score_timer[4] |= (0b101 << ((4 * i)));
      game_score_timer[3] |= (0b101 << ((4 * i)));
      game_score_timer[2] |= (0b111 << ((4 * i)));
      game_score_timer[1] |= (0b001 << ((4 * i)));
      game_score_timer[0] |= (0b001 << ((4 * i)));
    } else if (temp == 5) {
      game_score_timer[4] |= (0b111 << ((4 * i)));
      game_score_timer[3] |= (0b100 << ((4 * i)));
      game_score_timer[2] |= (0b110 << ((4 * i)));
      game_score_timer[1] |= (0b001 << ((4 * i)));
      game_score_timer[0] |= (0b110 << ((4 * i)));
    } else if (temp == 6) {
      game_score_timer[4] |= (0b111 << ((4 * i)));
      game_score_timer[3] |= (0b100 << ((4 * i)));
      game_score_timer[2] |= (0b111 << ((4 * i)));
      game_score_timer[1] |= (0b101 << ((4 * i)));
      game_score_timer[0] |= (0b111 << ((4 * i)));
    } else if (temp == 7) {
      game_score_timer[4] |= (0b111 << ((4 * i)));
      game_score_timer[3] |= (0b001 << ((4 * i)));
      game_score_timer[2] |= (0b001 << ((4 * i)));
      game_score_timer[1] |= (0b001 << ((4 * i)));
      game_score_timer[0] |= (0b001 << ((4 * i)));
    } else if (temp == 8) {
      game_score_timer[4] |= (0b111 << ((4 * i)));
      game_score_timer[3] |= (0b101 << ((4 * i)));
      game_score_timer[2] |= (0b111 << ((4 * i)));
      game_score_timer[1] |= (0b101 << ((4 * i)));
      game_score_timer[0] |= (0b111 << ((4 * i)));
    } else if (temp == 9) {
      game_score_timer[4] |= (0b111 << ((4 * i)));
      game_score_timer[3] |= (0b101 << ((4 * i)));
      game_score_timer[2] |= (0b111 << ((4 * i)));
      game_score_timer[1] |= (0b001 << ((4 * i)));
      game_score_timer[0] |= (0b001 << ((4 * i)));
    }
  }
}

void game_board_save_piece(uint8_t piece) {
  if (piece == 0) // clear save space
  {
    game_save_pieces_blue[1] = 0;
    game_save_pieces_blue[0] = 0;
    game_save_pieces_red[1] = 0;
    game_save_pieces_red[0] = 0;
    game_save_pieces_green[1] = 0;
    game_save_pieces_green[0] = 0;
  }
  if (piece == 1) // blue ricky
  {
    game_save_pieces_blue[1] = 0b0100;
    game_save_pieces_blue[0] = 0b0111;
    game_save_pieces_red[1] = 0;
    game_save_pieces_red[0] = 0;
    game_save_pieces_green[1] = 0;
    game_save_pieces_green[0] = 0;
  } else if (piece == 2) // cleaveland Z
  {
    game_save_pieces_blue[1] = 0;
    game_save_pieces_blue[0] = 0;
    game_save_pieces_red[1] = 0;
    game_save_pieces_red[0] = 0;
    game_save_pieces_green[1] = 0b0011;
    game_save_pieces_green[0] = 0b0110;
  } else if (piece == 3) // hero
  {
    game_save_pieces_blue[1] = 0;
    game_save_pieces_blue[0] = 0xF;
    game_save_pieces_red[1] = 0;
    game_save_pieces_red[0] = 0;
    game_save_pieces_green[1] = 0;
    game_save_pieces_green[0] = 0xF;
  } else if (piece == 4) // rhode_z
  {
    game_save_pieces_blue[1] = 0;
    game_save_pieces_blue[0] = 0;
    game_save_pieces_red[1] = 0b1100;
    game_save_pieces_red[0] = 0b0110;
    game_save_pieces_green[1] = 0;
    game_save_pieces_green[0] = 0;
  } else if (piece == 5) // teewee
  {
    game_save_pieces_blue[1] = 0b0010;
    game_save_pieces_blue[0] = 0b0111;
    game_save_pieces_red[1] = 0b0010;
    game_save_pieces_red[0] = 0b0111;
    game_save_pieces_green[1] = 0;
    game_save_pieces_green[0] = 0;
  } else if (piece == 6) // smashboy
  {
    game_save_pieces_blue[1] = 0;
    game_save_pieces_blue[0] = 0;
    game_save_pieces_red[1] = 0b0110;
    game_save_pieces_red[0] = 0b0110;
    game_save_pieces_green[1] = 0b0110;
    game_save_pieces_green[0] = 0b0110;
  } else if (piece == 7) // white ricky
  {
    game_save_pieces_blue[1] = 0b0001;
    game_save_pieces_blue[0] = 0b0111;
    game_save_pieces_red[1] = 0b0001;
    game_save_pieces_red[0] = 0b0111;
    game_save_pieces_green[1] = 0b0001;
    game_save_pieces_green[0] = 0b0111;
  }
}