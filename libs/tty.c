
#include "cursor.c"
#include "stack.c"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
//#error                                                                         \
    "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
// #error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

/* Hardware text mode color constants. */
enum vga_color {
  VGA_COLOR_BLACK = 0,
  VGA_COLOR_BLUE = 1,
  VGA_COLOR_GREEN = 2,
  VGA_COLOR_CYAN = 3,
  VGA_COLOR_RED = 4,
  VGA_COLOR_MAGENTA = 5,
  VGA_COLOR_ORANGE = 6,
  VGA_COLOR_LIGHT_GREY = 7,
  VGA_COLOR_DARK_GREY = 8,
  VGA_COLOR_LIGHT_BLUE = 9,
  VGA_COLOR_LIGHT_GREEN = 10,
  VGA_COLOR_LIGHT_CYAN = 11,
  VGA_COLOR_LIGHT_RED = 12,
  VGA_COLOR_LIGHT_MAGENTA = 13,
  VGA_COLOR_YELLOW = 14,
  VGA_COLOR_WHITE = 15,
};

enum vga_color_combos {
  VGA_NORMAL = VGA_COLOR_LIGHT_GREY | VGA_COLOR_BLACK << 4,
  VGA_ERROR = VGA_COLOR_WHITE | VGA_COLOR_RED << 4,
  VGA_SUCCESS = VGA_COLOR_WHITE | VGA_COLOR_GREEN << 4,
  VGA_WARNING = VGA_COLOR_WHITE | VGA_COLOR_YELLOW << 4,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
  return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
  return (uint16_t)uc | (uint16_t)color << 8;
  // bit 15: blink (makes character blink)
  // bits 8-14: color
  // bits 0-7: character
}

size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len])
    len++;
  return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t *terminal_buffer;

void terminal_initialize(void) {
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
  terminal_buffer = (uint16_t *)0xC03FF000;
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
  }
}

void terminal_setcolor(uint8_t color) { terminal_color = color; }

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = vga_entry(c, color);
}
void terminal_putchar(char c) {
  if (c == 10) {
    terminal_row += 1;
    terminal_column = -1;
    if (terminal_row == VGA_HEIGHT)
      terminal_row = 0;

  } else {
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
  }
  if (++terminal_column == VGA_WIDTH) {
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT)
      terminal_row = 0;
  }
  update_cursor(VGA_WIDTH, terminal_column - 1, terminal_row);
}

void terminal_write(const char *data, size_t size) {
  for (size_t i = 0; i < size; i++)
    terminal_putchar(data[i]);
}

inline void newline() { terminal_putchar(10); }
void terminal_writestring(const char *data) {
  terminal_write(data, strlen(data));
}
void terminal_writeint(uint32_t num) {
  if (num == 0) {
    terminal_putchar(0x30);
    return;
  }
  uint32_t len = 0;
  uint32_t clone = num;
  while (clone > 0) {
    clone /= 10;
    len += 1;
  }
  char numstr[len];
  len -= 1;
  while (len > 0) {
    numstr[len] = (num % 10) + 0x30;
    num /= 10;
    len -= 1;
  }
  numstr[len] = (num % 10) + 0x30;
  terminal_writestring(numstr);
}
inline void terminal_specialstring(const char *msg,
                                   enum vga_color_combos msgtype) {
  terminal_setcolor(msgtype);
  terminal_writestring(msg);
  terminal_setcolor(VGA_NORMAL);
}
inline void terminal_errorstring(const char *msg) {
  terminal_specialstring(msg, VGA_ERROR);
}
void terminal_successstring(const char *msg) {
  terminal_specialstring(msg, VGA_SUCCESS);
}
inline void terminal_specialint(uint32_t num, enum vga_color_combos msgtype) {
  terminal_setcolor(msgtype);
  terminal_writeint(num);
  terminal_setcolor(VGA_NORMAL);
}
inline void terminal_errorint(uint32_t num) {
  terminal_specialint(num, VGA_ERROR);
}
void terminal_uproot() { // scroll the terminal buffer upwards. deletes what is
                         // scrolled over
  for (size_t y = 1; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t og_idx = y * VGA_WIDTH + x;
      const size_t new_idx = (y - 1) * VGA_WIDTH + x;
      const uint8_t color = terminal_buffer[og_idx] >> 8;
      terminal_buffer[new_idx] = vga_entry(terminal_buffer[og_idx], color);
    }
  }
  terminal_row -= 1;
  update_cursor(VGA_WIDTH, terminal_column - 1, terminal_row);
}
// uint16_t *get_cursor_position() {
//   static uint16_t coords[2];
//   coords[0] = terminal_column - 1;
//   coords[1] = terminal_row;
//   return coords;
// }
uint16_t get_cursor_xcoord() { return terminal_column; }
uint16_t get_cursor_ycoord() { return terminal_row; }
