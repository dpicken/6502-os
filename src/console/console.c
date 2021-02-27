#include "console.h"

#include "lcd/init.h"
#include "lcd/io.h"

static char frame_buffer[40 * 4];

static unsigned char x_size;
static unsigned int frame_capacity;
static unsigned int frame_tail;
static unsigned int frame_head;
static unsigned char full;

void console_init_16x2(void) {
  x_size = 16;
  frame_capacity = 16 * 2;
  console_clear();
  lcd_set_16x2();
}

void console_init_40x4(void) {
  x_size = 40;
  frame_capacity = 40 * 4;
  console_clear();
  lcd_set_40x4();
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
  while (frame_tail != frame_head) {
    frame_buffer[frame_tail++] = ' ';
  }

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
  if (c == '\r') {
    frame_tail = frame_head - x_size;
    console_render();
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

void console_write(const char* buf, unsigned int count) {
  unsigned int i;

  for (i = 0; i != count; ++i) {
    console_putc(buf[i]);
  }
}
