#include "app.h"

#include "cc65/write.h"
#include "console/console.h"
#include "timer/timer.h"
#include "util/array.h"

#include <stdlib.h>

static const char* const wisdom_list[] = {
#include "wisdom_list.txt"
};

static int wisdom_index;

void wisdom_app_enter(void) {
  wisdom_play();
}

void wisdom_play(void) {
  wisdom_index = rand() % ARRAY_SIZE(wisdom_list);
  console_clear();
  wisdom_pretty_print(wisdom_list[wisdom_index]);
  timer_add_one_shot(wisdom_play, 5000);
}

void wisdom_pretty_print(const char* const wisdom) {
  unsigned char x_size = console_get_x_size();
  unsigned char x_pos = 0;
  const char* word_it = wisdom;
  const char* word_end = wisdom;

  int word_size;

  while (*word_end != '\0') {
    while (*word_end != ' ' && *word_end != '\0') ++word_end;
    word_size = word_end - word_it;
    if (word_size > (x_size - x_pos)) {
      fprintf(vidiprinter, "\n");
      x_pos = 0;
    }
    if (x_pos == 0 && *word_it == ' ') {
      ++word_it;
    }
    while (word_it != word_end) {
      fprintf(vidiprinter, "%c", *word_it);
      ++word_it;
      ++x_pos;
      if (x_pos == x_size) {
        x_pos = 0;
      }
    }
    if (*word_end == ' ') {
      ++word_end;
    }
  }
}
