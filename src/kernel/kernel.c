#include "lcd/init.h"
#include "lcd/putc.h"

void main() {
  lcd_init();
  lcd_putc('i');
}
