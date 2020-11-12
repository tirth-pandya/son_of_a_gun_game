#include "led_matrix.h"
#include "delay.h"
#include "gpio.h"
#include "lpc40xx.h"
#include "lpc_peripherals.h"

/**
 * r0,g0,b0 --> for first  32 rows
 * r1,g1,b1 --> for second 32 rows
 * a,b,c,d,e --> each row select - 2^5 = 32
 * oe --> inverted set low to enable display
 * latch --> set high to enable latch
 * clk
 **/
static gpio_s r0, g0, b0, r1, g1, b1, A, B, C, D, E, oe, latch, clk;

static void clear_pixel(uint8_t row, uint8_t col) {
  data_size pixel = ~((data_size)col);
  frame_buffer[row][RED_PLANE] &= pixel;
  frame_buffer[row][BLUE_PLANE] &= pixel;
  frame_buffer[row][GREEN_PLANE] &= pixel;
}

/*********************************************************************
 *********************PUBLIC FUNCTIONS********************************
 *********************************************************************/
void led_matrix_init(void) {
  // r0 = gpio__construct_with_function(GPIO__PORT_0, 6, GPIO__FUNCITON_0_IO_PIN);
  // b0 = gpio__construct_with_function(GPIO__PORT_0, 8, GPIO__FUNCITON_0_IO_PIN);
  // r1 = gpio__construct_with_function(GPIO__PORT_0, 26, GPIO__FUNCITON_0_IO_PIN);
  // b1 = gpio__construct_with_function(GPIO__PORT_1, 31, GPIO__FUNCITON_0_IO_PIN);
  // A = gpio__construct_with_function(GPIO__PORT_1, 20, GPIO__FUNCITON_0_IO_PIN);
  // C = gpio__construct_with_function(GPIO__PORT_1, 28, GPIO__FUNCITON_0_IO_PIN);
  // clk = gpio__construct_with_function(GPIO__PORT_2, 0, GPIO__FUNCITON_0_IO_PIN);
  // oe = gpio__construct_with_function(GPIO__PORT_2, 2, GPIO__FUNCITON_0_IO_PIN);

  // g0 = gpio__construct_with_function(GPIO__PORT_0, 7, GPIO__FUNCITON_0_IO_PIN);
  // g1 = gpio__construct_with_function(GPIO__PORT_0, 9, GPIO__FUNCITON_0_IO_PIN);
  // E = gpio__construct_with_function(GPIO__PORT_0, 25, GPIO__FUNCITON_0_IO_PIN);
  // B = gpio__construct_with_function(GPIO__PORT_1, 30, GPIO__FUNCITON_0_IO_PIN);
  // D = gpio__construct_with_function(GPIO__PORT_1, 23, GPIO__FUNCITON_0_IO_PIN);

  // latch = gpio__construct_with_function(GPIO__PORT_1, 29, GPIO__FUNCITON_0_IO_PIN);

  // r0 = gpio__construct_as_output(GPIO__PORT_0, 6);
  // b0 = gpio__construct_as_output(GPIO__PORT_0, 8);
  // r1 = gpio__construct_as_output(GPIO__PORT_0, 26);
  // b1 = gpio__construct_as_output(GPIO__PORT_1, 31);
  // A = gpio__construct_as_output(GPIO__PORT_1, 20);
  // C = gpio__construct_as_output(GPIO__PORT_1, 28);
  // clk = gpio__construct_as_output(GPIO__PORT_2, 0);
  // oe = gpio__construct_as_output(GPIO__PORT_2, 2);

  // g0 = gpio__construct_as_output(GPIO__PORT_0, 7);
  // g1 = gpio__construct_as_output(GPIO__PORT_0, 9);
  // E = gpio__construct_as_output(GPIO__PORT_0, 25);
  // B = gpio__construct_as_output(GPIO__PORT_1, 30);
  // D = gpio__construct_as_output(GPIO__PORT_1, 23);

  // latch = gpio__construct_as_output(GPIO__PORT_1, 29);

  r0 = gpio__construct_with_function(GPIO__PORT_2, 0, GPIO__FUNCITON_0_IO_PIN);
  g0 = gpio__construct_with_function(GPIO__PORT_2, 1, GPIO__FUNCITON_0_IO_PIN);
  b0 = gpio__construct_with_function(GPIO__PORT_2, 2, GPIO__FUNCITON_0_IO_PIN);
  r1 = gpio__construct_with_function(GPIO__PORT_2, 4, GPIO__FUNCITON_0_IO_PIN);
  g1 = gpio__construct_with_function(GPIO__PORT_2, 5, GPIO__FUNCITON_0_IO_PIN);
  b1 = gpio__construct_with_function(GPIO__PORT_2, 6, GPIO__FUNCITON_0_IO_PIN);

  A = gpio__construct_with_function(GPIO__PORT_2, 7, GPIO__FUNCITON_0_IO_PIN);
  B = gpio__construct_with_function(GPIO__PORT_2, 8, GPIO__FUNCITON_0_IO_PIN);
  C = gpio__construct_with_function(GPIO__PORT_2, 9, GPIO__FUNCITON_0_IO_PIN);
  D = gpio__construct_with_function(GPIO__PORT_0, 16, GPIO__FUNCITON_0_IO_PIN);
  E = gpio__construct_with_function(GPIO__PORT_0, 15, GPIO__FUNCITON_0_IO_PIN);

  oe = gpio__construct_with_function(GPIO__PORT_1, 20, GPIO__FUNCITON_0_IO_PIN);
  latch = gpio__construct_with_function(GPIO__PORT_1, 23, GPIO__FUNCITON_0_IO_PIN);
  clk = gpio__construct_with_function(GPIO__PORT_1, 28, GPIO__FUNCITON_0_IO_PIN);

  r0 = gpio__construct_as_output(GPIO__PORT_2, 0);
  g0 = gpio__construct_as_output(GPIO__PORT_2, 1);
  b0 = gpio__construct_as_output(GPIO__PORT_2, 2);
  r1 = gpio__construct_as_output(GPIO__PORT_2, 4);
  g1 = gpio__construct_as_output(GPIO__PORT_2, 5);
  b1 = gpio__construct_as_output(GPIO__PORT_2, 6);

  A = gpio__construct_as_output(GPIO__PORT_2, 7);
  B = gpio__construct_as_output(GPIO__PORT_2, 8);
  C = gpio__construct_as_output(GPIO__PORT_2, 9);
  D = gpio__construct_as_output(GPIO__PORT_0, 16);
  E = gpio__construct_as_output(GPIO__PORT_0, 15);

  oe = gpio__construct_as_output(GPIO__PORT_1, 20);
  latch = gpio__construct_as_output(GPIO__PORT_1, 23);
  clk = gpio__construct_as_output(GPIO__PORT_1, 28);

  gpio__reset(A);
  gpio__reset(B);
  gpio__reset(C);
  gpio__reset(D);
  gpio__reset(E);
  gpio__reset(r0);
  gpio__reset(g0);
  gpio__reset(b0);
  gpio__reset(r1);
  gpio__reset(g1);
  gpio__reset(b1);

  led_matrix__disable_display();
  led_matrix__disable_latch();
  gpio__reset(clk);
}

void led_matrix__enable_display() { gpio__reset(oe); }

void led_matrix__disable_display() { gpio__set(oe); }

void led_matrix__enable_latch() { gpio__set(latch); }

void led_matrix__disable_latch() { gpio__reset(latch); }

void led_matrix__select_row(int row) {
  (row & 0x01) ? gpio__set(A) : gpio__reset(A);
  (row & 0x02) ? gpio__set(B) : gpio__reset(B);
  (row & 0x04) ? gpio__set(C) : gpio__reset(C);
  (row & 0x08) ? gpio__set(D) : gpio__reset(D);
  (row & 0x10) ? gpio__set(E) : gpio__reset(E);
}

void led_matrix__clear_pixel(int row, int column) {
  data_size pixel = ~((uint64_t)1 << column);
  frame_buffer[row][RED_PLANE] &= pixel;
  frame_buffer[row][GREEN_PLANE] &= pixel;
  frame_buffer[row][BLUE_PLANE] &= pixel;
}

void led_matrix__set_pixel(int row, int column, led_matrix__color_e color) {
  data_size pixel = ((data_size)1 << column);
  if (color & BLUE_COLOR_BIT) {
    frame_buffer[row][BLUE_PLANE] |= pixel;
  }
  if (color & GREEN_COLOR_BIT) {
    frame_buffer[row][GREEN_PLANE] |= pixel;
  }
  if (color & RED_COLOR_BIT) {
    frame_buffer[row][RED_PLANE] |= pixel;
  }
  if (color == OFF) {
    clear_pixel(row, column);
  }
}

void led_matrix__set_row_data(int row, led_matrix__color_e color, data_size data) {
  if (color & BLUE_COLOR_BIT) {
    frame_buffer[row][BLUE_PLANE] = data;
  }
  if (color & GREEN_COLOR_BIT) {
    frame_buffer[row][GREEN_PLANE] = data;
  }
  if (color & RED_COLOR_BIT) {
    frame_buffer[row][RED_PLANE] = data;
  }
}

void led_matrix__fill_data_buffer(data_size data, led_matrix__color_e color) {
  for (int row = 0; row < 64; row++) {
    led_matrix__set_row_data(row, color, data);
  }
}

void led_matrix__fill_data_buffer_till_row(data_size data, uint8_t start_row, uint8_t till_row,
                                           led_matrix__color_e color) {
  for (int row = start_row; row < till_row; row++) {
    led_matrix__set_row_data(row, color, data);
  }
}

void led_matrix__clear_data_buffer() { memset(frame_buffer, 0, sizeof(frame_buffer)); }

void led_matrix__update_display() {
  for (int i = 0; i < 64; i++) {
    led_matrix__disable_display();
    led_matrix__disable_latch();
    led_matrix__select_row(i); // will select i and i + 32 rows at same time

    for (int j = 63; j >= 0; j--) { // shift data with MSB getting shifted-in first
      ((frame_buffer[i][RED_PLANE] >> j) & 1) ? gpio__set(r0) : gpio__reset(r0);
      ((frame_buffer[i][GREEN_PLANE] >> j) & 1) ? gpio__set(g0) : gpio__reset(g0);
      ((frame_buffer[i][BLUE_PLANE] >> j) & 1) ? gpio__set(b0) : gpio__reset(b0);
      // if ((frame_buffer[i][RED_PLANE] >> j) || (frame_buffer[i][GREEN_PLANE] >> j) ||
      //     (frame_buffer[i][BLUE_PLANE] >> j))
      //   printf("1");
      ((frame_buffer[i + 32][RED_PLANE] >> j) & 1) ? gpio__set(r1) : gpio__reset(r1);
      ((frame_buffer[i + 32][GREEN_PLANE] >> j) & 1) ? gpio__set(g1) : gpio__reset(g1);
      ((frame_buffer[i + 32][BLUE_PLANE] >> j) & 1) ? gpio__set(b1) : gpio__reset(b1);
      // if ((frame_buffer[i + 32][RED_PLANE] >> j) || (frame_buffer[i + 32][GREEN_PLANE] >> j) ||
      //     (frame_buffer[i + 32][BLUE_PLANE] >> j))
      //   printf("1");
      gpio__set(clk);
      gpio__reset(clk); // shift in all 3 color bits at once for top half/bottom half registers
      led_matrix__enable_latch();
      led_matrix__disable_latch();
    }
    // printf("\n");
    // at this point, all 3 shift registers should be filled with corresponding row data in frame_buffer
    led_matrix__enable_latch(); // push shift register contents down to output registers
    led_matrix__enable_display();
    delay__us(50);
  }
  // fprintf(stderr, "-------------------\n");
  led_matrix__disable_display();
}