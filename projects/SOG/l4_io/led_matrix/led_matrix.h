#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define data_size uint64_t

typedef enum { // RGB (000 => Off)
  OFF,
  BLUE,
  GREEN,
  CYAN,
  RED,
  MAGENTA,
  YELLOW,
  WHITE,
  FILL
} led_matrix__color_e;

typedef enum { RED_PLANE, GREEN_PLANE, BLUE_PLANE, FRIEND_PLANE, ENEMY_PLANE, LIFE_PLANE } led_matrix__color_plane_e;

typedef enum { BLUE_COLOR_BIT = 0x1, GREEN_COLOR_BIT = 0x2, RED_COLOR_BIT = 0x4 } led_pixel_color_bit_e;

/**
 * 2 Dimensional array of 64 bits (each bit correspond to columns in a given row)
 * row of array -> Row of LED matrix
 * columns of array -> color bit of each pixel (R/G/B)
 */
data_size frame_buffer[64][6];

/**
 * Initializes all assigned GPIOs for LED matrix
 */
void led_matrix_init(void);

/**
 * Enables or disabled LED matrix
 */
void led_matrix__enable_display();
void led_matrix__disable_display();

/**
 * Enables or disabled latch of the LEDmatrix
 */
void led_matrix__enable_latch();
void led_matrix__disable_latch();

/**
 * Configures the input address signal as per the input row. For 64x64 LED matrix,
 * two rows will be configured (viz. 1. row, 2. row+32)
 * @param row The row index to be selected
 */
void led_matrix__select_row(int row);

/**
 * Sets the R,G,B signal corresponding to given pixel index (row & column)
 * @param row Row index of the given pixel
 * @param column Column index of the given pixel
 * @param color Color to be set for the given pixel
 * this has column flipped compared to other object functions
 */
void led_matrix__set_pixel(int row, int column, led_matrix__color_e color);

/**
 * Reset the R,G,B signal corresponding to given pixel index
 * @param row Row index of the pixel to be cleared
 * @param column Column index of the pixel to be cleared
 */
void led_matrix__clear_pixel(int row, int column);

/**
 * Update the LED matrix signals as per the currently configured pixel buffer.
 * This API will be responsible for feeding the configured data. It should be called
 * at a configured LED refresh frequency
 */
void led_matrix__update_display();

/**
 * Update the data (bitmap of all 64 LEDs in a given row) and color signals corresponding to
 * given row index
 * @param row Row index of LED matrix corresponding to which data need to be updated
 * @param color R/G/B singal update data
 * @param data LED bitmap for all LEDs in a given row
 */
void led_matrix__set_row_data(int row, led_matrix__color_e color, data_size data);

/**
 * Fill given data for all of the rows with given color signals
 * @param data LED bitmap need to be updated for all rows in LED matrix
 * @param color Color signals need to be updated for all pixels
 */
void led_matrix__fill_data_buffer(data_size data, led_matrix__color_e color);

void led_matrix__clear_data_buffer();

void led_matrix__fill_data_buffer_till_row(data_size data, uint8_t start_row, uint8_t till_row,
                                           led_matrix__color_e color);
