#include "display.h"

#include "driver/hd44780/hd44780.h"
#include "driver/us2066/us2066.h"

#include "kernel/delay.h"
#include "util/algorithm.h"

#include <ctype.h>
#include <limits.h>

const display_driver* display;

static char frame_buffer[40 * 4];

static unsigned char x_size;
static unsigned char frame_capacity;
static unsigned char frame_tail;
static unsigned char frame_head;
static unsigned char full;

void display_init_hd44780(hw_register via_port, hw_register via_ddr) {
  hd44780_init(via_port, via_ddr);
  display = &display_hd44780;
  display_set_resolution(20, 4);
}

void display_init_us2066(hw_register via_port, hw_register via_ddr) {
  us2066_init(via_port, via_ddr);
  display = &display_us2066;
  display_set_resolution(20, 4);
}

void display_set_resolution(unsigned char x, unsigned char y) {
  x_size = x;
  frame_capacity = x * y;
  display->set_resolution(x, y);
  display_clear();
}

unsigned char display_get_x_size(void) {
  return x_size;
}

void display_clear(void) {
  unsigned char i;

  for (i = 0; i < frame_capacity; ++i) {
    frame_buffer[i] = ' ';
  }

  frame_tail = 0;
  frame_head = frame_tail + x_size;
  full = 0;

  display->clear();
  display_render();
}

void display_render(void) {
  display->set_pos_home();
  if (full) {
    display_write_direct(frame_buffer + frame_head, frame_capacity - frame_head);
    display_write_direct(frame_buffer, frame_head);
    display->set_pos(frame_capacity - x_size);
  } else {
    display_write_direct(frame_buffer, frame_tail);
  }
}

void display_write_direct(const char* it, unsigned char count) {
  const char* end = it + count;

  while (it != end) {
    display->putchar(*it);
    ++it;
  }
}

void display_putc(char c) {
  unsigned char offset_from_line_begin;

  if (c == '\r') {
    offset_from_line_begin = x_size - (frame_head - frame_tail);
    frame_tail = frame_head - x_size;
    display->set_pos(display->get_pos() - offset_from_line_begin);
    return;
  }

  if (c == '\n') {
    display_new_line();
    return;
  }

  if (frame_tail == frame_head) {
    display_new_line();
  }

  frame_buffer[frame_tail++] = c;
  display->putchar(c);
}

void display_new_line(void) {
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

  display_render();
}

void display_vidiprinter_putc(char c) {
  display_putc(c);
  display_vidiprinter_delay(c);
}

void display_vidiprinter_delay(char c) {
  delay_ms(c == ',' || c == ';' || c == ':' || c == '.' || c == '?' ? 400 : 80);
}
