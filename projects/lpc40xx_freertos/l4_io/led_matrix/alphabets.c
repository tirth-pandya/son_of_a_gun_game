#include "alphabets.h"
#include <stdint.h>

u_int8_t a[6] = {0x00, 0x7c, 0x44, 0x44, 0x7c, 0x44}, b[6] = {0x00, 0x7c, 0x44, 0x78, 0x44, 0x7c},
         c[6] = {0x00, 0x7c, 0x40, 0x40, 0x40, 0x7c}, d[6] = {0x00, 0x78, 0x44, 0x44, 0x44, 0x78},
         e[6] = {0x00, 0x7c, 0x40, 0x78, 0x40, 0x7c}, f[6] = {0x00, 0x7c, 0x40, 0x70, 0x40, 0x40},
         g[6] = {0x00, 0x7c, 0x40, 0x4c, 0x44, 0x7c}, h[6] = {0x00, 0x44, 0x44, 0x7c, 0x44, 0x44},
         i[6] = {0x00, 0x7c, 0x10, 0x10, 0x10, 0x7c}, j[6] = {0x00, 0x0c, 0x04, 0x04, 0x44, 0x7c},
         k[6] = {0x00, 0x44, 0x48, 0x70, 0x48, 0x44}, l[6] = {0x00, 0x40, 0x40, 0x40, 0x40, 0x7c},
         m[6] = {0x00, 0x44, 0x6c, 0x54, 0x44, 0x44}, n[6] = {0x00, 0x44, 0x64, 0x54, 0x4c, 0x44},
         o[6] = {0x00, 0x38, 0x44, 0x44, 0x44, 0x38}, p[6] = {0x00, 0x78, 0x44, 0x78, 0x40, 0x40},
         q[6] = {0x00, 0x7c, 0x44, 0x44, 0x7c, 0x10}, r[6] = {0x00, 0x78, 0x44, 0x78, 0x44, 0x44},
         s[6] = {0x00, 0x3c, 0x40, 0x7c, 0x04, 0x78}, t[6] = {0x00, 0x7c, 0x10, 0x10, 0x10, 0x10},
         u[6] = {0x00, 0x44, 0x44, 0x44, 0x44, 0x7c}, v[6] = {0x00, 0x44, 0x44, 0x28, 0x28, 0x10},
         w[6] = {0x00, 0x44, 0x44, 0x54, 0x54, 0x28}, x[6] = {0x00, 0x44, 0x28, 0x10, 0x28, 0x44},
         y[6] = {0x00, 0x44, 0x44, 0x28, 0x10, 0x10}, z[6] = {0x00, 0x7c, 0x08, 0x10, 0x20, 0x7c},

         ze[6] = {0x00, 0x7c, 0x4c, 0x54, 0x64, 0x7c}, on[6] = {0x00, 0x10, 0x30, 0x10, 0x10, 0x38},
         tw[6] = {0x00, 0x78, 0x04, 0x38, 0x40, 0x7c}, th[6] = {0x00, 0x7c, 0x04, 0x38, 0x04, 0x7c},
         fo[6] = {0x00, 0x40, 0x40, 0x50, 0x7c, 0x10}, fi[6] = {0x00, 0x7c, 0x40, 0x78, 0x04, 0x78},
         si[6] = {0x00, 0x7c, 0x40, 0x7c, 0x44, 0x7c}, se[6] = {0x00, 0x7c, 0x04, 0x08, 0x10, 0x10},
         ei[6] = {0x00, 0x7c, 0x44, 0x7c, 0x44, 0x7c}, ni[6] = {0x00, 0x7c, 0x44, 0x7c, 0x04, 0x7c};

void print_char(char *input_char, uint8_t row, uint8_t column, led_matrix__color_e color) {
  int it;
  uint64_t row_buff = 0, temp_row;
  uint8_t len, spacer = 6;
  int shifter;
  char char_finder;

  if (row > 59)
    row = 59;
  for (it = 0; input_char[it] != 0; it++)
    ;
  len = it;
  for (u_int8_t iterator = 0; iterator < 6; iterator++) {

    for (uint8_t char_iterator = 0; char_iterator < len; char_iterator++) {

      if (char_iterator == (len - 1)) {

        spacer = 0;
      }
      char_finder = input_char[char_iterator];
      switch (char_finder) {

      case 'a':
        row_buff |= a[iterator];
        row_buff = row_buff << spacer;
        break;

      case 'b':
        row_buff |= b[iterator];
        row_buff = row_buff << spacer;
        break;

      case 'c':
        row_buff |= c[iterator];
        row_buff = row_buff << spacer;
        break;

      case 'd':
        row_buff |= d[iterator];
        row_buff = row_buff << spacer;
        break;

      case 'e':
        row_buff |= e[iterator];
        row_buff = row_buff << spacer;
        break;

      case 'f':
        row_buff |= f[iterator];
        row_buff = row_buff << spacer;
        break;

      case 'g':
        row_buff |= g[iterator];
        row_buff = row_buff << spacer;
        break;

      case 'h':
        row_buff |= h[iterator];
        row_buff = row_buff << spacer;
        break;

      case 'i':
        row_buff |= i[iterator];
        row_buff = row_buff << spacer;
        break;

      case 'j':
        row_buff |= j[iterator];
        row_buff = row_buff << spacer;
        break;

      case 'k':
        row_buff |= k[iterator];
        row_buff = row_buff << spacer;
        break;

      case 'l':
        row_buff |= l[iterator];
        row_buff = row_buff << spacer;
        break;

      case 'm':
        row_buff |= m[iterator];
        row_buff = row_buff << spacer;
        break;

      case 'n':
        row_buff |= n[iterator];
        row_buff = row_buff << spacer;
        break;

      case 'o':
        row_buff |= o[iterator];
        row_buff = row_buff << spacer;
        break;

      case 'p':
        row_buff |= p[iterator];
        row_buff = row_buff << spacer;
        break;

      case 'q':
        row_buff |= q[iterator];
        row_buff = row_buff << spacer;
        break;

      case 'r':
        row_buff |= r[iterator];
        row_buff = row_buff << spacer;
        break;

      case 's':
        row_buff |= s[iterator];
        row_buff = row_buff << spacer;
        break;

      case 't':
        row_buff |= t[iterator];
        row_buff = row_buff << spacer;
        break;

      case 'u':
        row_buff |= u[iterator];
        row_buff = row_buff << spacer;
        break;

      case 'v':
        row_buff |= v[iterator];
        row_buff = row_buff << spacer;
        break;

      case 'w':
        row_buff |= w[iterator];
        row_buff = row_buff << spacer;
        break;

      case 'x':
        row_buff |= x[iterator];
        row_buff = row_buff << spacer;
        break;

      case 'y':
        row_buff |= y[iterator];
        row_buff = row_buff << spacer;
        break;

      case 'z':
        row_buff |= z[iterator];
        row_buff = row_buff << spacer;
        break;

      case '1':
        row_buff |= on[iterator];
        row_buff = row_buff << spacer;
        break;

      case '2':
        row_buff |= tw[iterator];
        row_buff = row_buff << spacer;
        break;

      case '3':
        row_buff |= th[iterator];
        row_buff = row_buff << spacer;
        break;

      case '4':
        row_buff |= fo[iterator];
        row_buff = row_buff << spacer;
        break;

      case '5':
        row_buff |= fi[iterator];
        row_buff = row_buff << spacer;
        break;

      case '6':
        row_buff |= si[iterator];
        row_buff = row_buff << spacer;
        break;

      case '7':
        row_buff |= se[iterator];
        row_buff = row_buff << spacer;
        break;

      case '8':
        row_buff |= ei[iterator];
        row_buff = row_buff << spacer;
        break;

      case '9':
        row_buff |= ni[iterator];
        row_buff = row_buff << spacer;
        break;

      case '0':
        row_buff |= ze[iterator];
        row_buff = row_buff << spacer;
        break;

      default:;
        // row_buff |= x[iterator];
        // row_buff = row_buff << 5;
      }
    }
    // row_buff = row_buff >> ((len-1) * spacer);
    // row_buff = row_buff << column;
    if (spacer == 0)
      spacer = 6;
    shifter = 63 - column - ((len)*spacer);
    if (shifter < 0)
      row_buff = row_buff >> 8;
    else {
      row_buff = row_buff << shifter;
    }

    temp_row = row_buff;

    temp_row = ~temp_row;

    frame_buffer[row + iterator - 1][BLUE_PLANE] &= temp_row;
    frame_buffer[row + iterator - 1][RED_PLANE] &= temp_row;
    frame_buffer[row + iterator - 1][GREEN_PLANE] &= temp_row;

    temp_row = ~temp_row;

    switch (color) {
    case OFF:
      temp_row = 0;
      frame_buffer[row + iterator - 1][BLUE_PLANE] |= temp_row;
      frame_buffer[row + iterator - 1][RED_PLANE] |= temp_row;
      frame_buffer[row + iterator - 1][GREEN_PLANE] |= temp_row;
      break;
    case BLUE:
      frame_buffer[row + iterator - 1][BLUE_PLANE] |= temp_row;
      break;
    case GREEN:
      frame_buffer[row + iterator - 1][GREEN_PLANE] |= temp_row;
      break;
    case CYAN:
      frame_buffer[row + iterator - 1][BLUE_PLANE] |= temp_row;
      frame_buffer[row + iterator - 1][GREEN_PLANE] |= temp_row;
      break;
    case RED:
      frame_buffer[row + iterator - 1][RED_PLANE] |= temp_row;
      break;
    case MAGENTA:
      frame_buffer[row + iterator - 1][BLUE_PLANE] |= temp_row;
      frame_buffer[row + iterator - 1][RED_PLANE] |= temp_row;
      break;
    case YELLOW:
      frame_buffer[row + iterator - 1][RED_PLANE] |= temp_row;
      frame_buffer[row + iterator - 1][GREEN_PLANE] |= temp_row;
      break;
    case WHITE:
      frame_buffer[row + iterator - 1][BLUE_PLANE] |= temp_row;
      frame_buffer[row + iterator - 1][RED_PLANE] |= temp_row;
      frame_buffer[row + iterator - 1][GREEN_PLANE] |= temp_row;
      break;
    default:
      break;
    }
    
    row_buff = 0;
  }
}