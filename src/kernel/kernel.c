#include "button/init.h"
#include "lcd/init.h"
#include "lcd/putc.h"

void kernel_out(const char* str) {
  while (*str != '\0') {
    lcd_putc(*str++);
  }
}

void kernel_button_poll_loop() {
  for(;;) {
    unsigned char button_bits = hw_button_get();
    if (button_bits & HW_BUTTON_0) {
      lcd_putc('0');
    }
    if (button_bits & HW_BUTTON_1) {
      lcd_putc('1');
    }
    if (button_bits & HW_BUTTON_2) {
      lcd_putc('2');
    }
    if (button_bits & HW_BUTTON_3) {
      lcd_putc('3');
    }
    if (button_bits & HW_BUTTON_4) {
      lcd_putc('4');
    }
  }
}

void main() {
  lcd_init();
  kernel_out("[lcd_init] done                         ");
  button_init();
  kernel_out("[btn_init] done                         ");
  kernel_button_poll_loop();
}
