/**
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright 2020, Anuradha Weeraman
 */

#include "tty.h"

#ifdef CONFIG_SERIAL
#include "serial.h"
#endif

cell *matrix = (cell *) 0xb8000;

static uint32_t cur_x = 0;
static uint32_t cur_y = 0;

void scroll() {
  for (uint32_t i=0; i<(TERMINAL_ROWS-1); i++) {
    for (uint32_t j=0; j<TERMINAL_COLS; j++) {
      matrix[(i*TERMINAL_COLS) + j].ch    = matrix[(i+1)*TERMINAL_COLS + j].ch;
      matrix[(i*TERMINAL_COLS) + j].clr   = matrix[(i+1)*TERMINAL_COLS + j].clr;
      matrix[(i+1)*TERMINAL_COLS + j].ch  = 0;
      matrix[(i+1)*TERMINAL_COLS + j].clr = 0;
    }
  }
}

void print_char(uint8_t ch) {
  if (cur_y >= TERMINAL_COLS) {
    cur_x++;
    cur_y = 0;
  }
  if (cur_x >= TERMINAL_ROWS) {
    scroll();
    cur_x = TERMINAL_ROWS-1;
    cur_y = 0;
  }
  if (ch == '\n' || ch == '\r') {
    cur_x++;
    cur_y = 0;
    if (cur_x >= TERMINAL_ROWS) {
      scroll();
      cur_x = TERMINAL_ROWS-1;
    }
#ifdef CONFIG_SERIAL
    write_serial('\r');
    write_serial('\n');
#endif
    return;
  }
  matrix[(cur_x*TERMINAL_COLS) + cur_y++] = (cell) {
    .ch = ch,
    .clr = CLR_WHITE
  };
#ifdef CONFIG_SERIAL
  write_serial(ch);
#endif
}

void clear(void) {
  for (uint32_t i=0; i<(TERMINAL_ROWS*TERMINAL_COLS); i++) {
    matrix[i].ch = 0;
    matrix[i].clr = 0;
  }
}

void print(char *str) {
  for (uint32_t i=0; str[i] != '\0'; i++) {
    print_char(str[i]);
  }
}

void println(char *str) {
  print(str);
  print("\n");
}