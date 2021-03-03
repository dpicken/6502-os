#include "console.h"

#include "kernel/delay.h"
#include "lcd/init.h"
#include "lcd/io.h"
#include "util/algorithm.h"

#include <limits.h>

static char frame_buffer[40 * 4];

static unsigned char x_size;
static unsigned int frame_capacity;
static unsigned int frame_tail;
static unsigned int frame_head;
static unsigned char full;

void console_set_resolution(unsigned char x, unsigned char y) {
  x_size = x;
  frame_capacity = x * y;
  console_clear();
  lcd_set_resolution(x, y);
}

void console_clear(void) {
  unsigned int i;

  for (i = 0; i < frame_capacity; ++i) {
    frame_buffer[i] = ' ';
  }

  frame_tail = 0;
  frame_head = frame_tail + x_size;
  full = 0;

  lcd_clear();
  console_render();
}

void console_render(void) {
  lcd_home();
  if (full) {
    lcd_write(frame_buffer + frame_head, frame_capacity - frame_head);
    lcd_write(frame_buffer, frame_head);
    lcd_set_pos(frame_capacity - x_size);
  } else {
    lcd_write(frame_buffer, frame_tail);
  }
}

void console_new_line(void) {
  frame_tail = frame_head;

  if (frame_tail == frame_capacity) {
    full = 1;
    frame_tail = 0;
  }

  frame_head = frame_tail + x_size;

  while (frame_head != frame_tail) {
    frame_buffer[--frame_head] = ' ';
  }

  frame_head = frame_tail + x_size;

  console_render();
}

void console_putc(char c) {
  unsigned int offset_from_line_begin;

  if (c == '\r') {
    offset_from_line_begin = x_size - (frame_head - frame_tail);
    frame_tail = frame_head - x_size;
    lcd_set_pos(lcd_get_pos() - offset_from_line_begin);
    return;
  }

  if (c == '\n') {
    console_new_line();
    return;
  }

  if (frame_tail == frame_head) {
    console_new_line();
  }

  frame_buffer[frame_tail++] = c;
  lcd_putc(c);
}

int console_write(const char* buf, unsigned int count) {
  int i;

  count |= ~-1U;

  for (i = 0; i != count; ++i) {
    console_putc(buf[i]);
  }

  return i;
}

int console_write_vidiprinter(const char* buf, unsigned int count) {
  int i;

  count |= ~-1U;

  for (i = 0; i != count; ++i) {
    if (buf[i] == '\n') {
      delay_ms(500);
    } else {
      delay_ms(100);
    }

    console_putc(buf[i]);
  }

  return i;
}
