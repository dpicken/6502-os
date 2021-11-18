#include "panic.h"

#include "controller/controller.h"
#include "kernel/delay.h"
#include "lcd/lcd.h"
#include "timer/timer.h"

#include <string.h>

static unsigned char panicking;

void panic(const char* const message) {
  unsigned char led_on = 0;

  if (panicking) {
    for (;;) {
    }
  }

  panicking = 1;

  lcd->clear();
  lcd_write_str("panic: ");
  lcd_write_str(message);

  for (;;) {
    delay_ms(1000);
    led_on = !led_on;
    if (led_on) {
      controller_led_off();
    } else {
      controller_led_on();
    }
  }
}
