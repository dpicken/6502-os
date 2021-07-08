#include "panic.h"

#include "timer/timer.h"
#include "lcd/lcd.h"
#include "led/led.h"

#include <string.h>

static unsigned char panicking;

void panic(const char* const message) {
  if (panicking) {
    for (;;) {
    }
  }

  panicking = 1;

  led_on();

  lcd->clear();
  lcd_write_str("panic: ");
  lcd_write_str(message);

  for (;;) {
  }
}
