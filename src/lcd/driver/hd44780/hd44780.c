#include "hd44780.h"

#include "hd44780_constants.h"

#include "kernel/delay.h"
#include "via/via.h"

#define HD44780_XSIZE_MAX 40
#define HD44780_E1_POS_END 80
#define HD44780_E2_POS_END 160

#define HD44780_LINE_1_DDRAM_ADDRESS_BASE 0x00
#define HD44780_LINE_2_DDRAM_ADDRESS_BASE 0x40

#define HD44780_LINE_1_DDRAM_ADDRESS_END (HD44780_LINE_1_DDRAM_ADDRESS_BASE + HD44780_XSIZE_MAX)
#define HD44780_LINE_2_DDRAM_ADDRESS_END (HD44780_LINE_2_DDRAM_ADDRESS_BASE + HD44780_XSIZE_MAX)

static hw_register hd44780_via_port;
static hw_register hd44780_via_ddr;

static unsigned char lcd_xsize = HD44780_XSIZE_MAX;
static unsigned char lcd_pos_end = HD44780_E2_POS_END;
static unsigned char lcd_pos;

const lcd_driver lcd_hd44780 = {
  hd44780_init,
  hd44780_set_resolution,
  hd44780_get_resolution_x,
  hd44780_get_resolution_y,
  hd44780_get_resolution_cell_count,
  hd44780_clear,
  hd44780_set_pos_home,
  hd44780_set_pos,
  hd44780_get_pos,
  hd44780_display_shift_left,
  hd44780_display_shift_right,
  hd44780_display_off,
  hd44780_display_on,
  hd44780_contrast_decrement,
  hd44780_contrast_increment,
  hd44780_orientation_default,
  hd44780_orientation_rotated,

  hd44780_putchar
};

void hd44780_init(hw_register via_port, hw_register via_ddr) {
  hd44780_via_port = via_port;
  hd44780_via_ddr = via_ddr;

  VIA_DATA_DIRECTION_SET_WRITE(hd44780_via_ddr);

  hd44780_via_write_control_init_hi(HD44780_INSTRUCTION_FUNCTION_SET
      | HD44780_FUNCTION_SET_DATA_LENGTH_8BIT
      , HD44780_VIA_CONTROL_E1 | HD44780_VIA_CONTROL_E2);
  hd44780_via_write_control_init_hi(HD44780_INSTRUCTION_FUNCTION_SET
      | HD44780_FUNCTION_SET_DATA_LENGTH_8BIT
      , HD44780_VIA_CONTROL_E1 | HD44780_VIA_CONTROL_E2);
  hd44780_via_write_control_init_hi(HD44780_INSTRUCTION_FUNCTION_SET
      | HD44780_FUNCTION_SET_DATA_LENGTH_8BIT
      , HD44780_VIA_CONTROL_E1 | HD44780_VIA_CONTROL_E2);
  hd44780_via_write_control_init_hi(HD44780_INSTRUCTION_FUNCTION_SET
      | HD44780_FUNCTION_SET_DATA_LENGTH_4BIT
      , HD44780_VIA_CONTROL_E1 | HD44780_VIA_CONTROL_E2);

  hd44780_via_write_control(HD44780_INSTRUCTION_FUNCTION_SET
      | HD44780_FUNCTION_SET_DATA_LENGTH_4BIT
      | HD44780_FUNCTION_SET_DISPLAY_LINES_2
      , HD44780_VIA_CONTROL_E1 | HD44780_VIA_CONTROL_E2);

  hd44780_via_write_control(HD44780_INSTRUCTION_ENTRY_MODE_SET
      | HD44780_ENTRY_MODE_SET_INCREMENT
      | HD44780_ENTRY_MODE_SET_SHIFT_DISPLAY_OFF
      , HD44780_VIA_CONTROL_E1 | HD44780_VIA_CONTROL_E2);
  hd44780_via_write_control(HD44780_INSTRUCTION_DISPLAY_CONTROL
      | HD44780_DISPLAY_CONTROL_ON
      | HD44780_DISPLAY_CONTROL_CURSOR_OFF
      | HD44780_DISPLAY_CONTROL_CURSOR_BLINK_OFF
      , HD44780_VIA_CONTROL_E1 | HD44780_VIA_CONTROL_E2);
  hd44780_clear();
}

void hd44780_set_resolution(unsigned char x, unsigned char y) {
  lcd_xsize = x;
  lcd_pos_end = x * y;
  lcd_pos = 0;
}

unsigned char hd44780_get_resolution_x(void) {
  return lcd_xsize;
}

unsigned char hd44780_get_resolution_y(void) {
  return lcd_pos_end / lcd_xsize;
}

unsigned char hd44780_get_resolution_cell_count(void) {
  return lcd_pos_end;
}

void hd44780_clear(void) {
  hd44780_via_write_control(HD44780_INSTRUCTION_CLEAR_DISPLAY, HD44780_VIA_CONTROL_E1 | HD44780_VIA_CONTROL_E2);
  lcd_pos = 0;
}

void hd44780_set_pos_home(void) {
  hd44780_via_write_control(HD44780_INSTRUCTION_RETURN_HOME, HD44780_VIA_CONTROL_E1 | HD44780_VIA_CONTROL_E2);
  lcd_pos = 0;
}

void hd44780_set_pos(unsigned char pos) {
  unsigned char x = pos % lcd_xsize;
  unsigned char y = pos / lcd_xsize;

  if (lcd_pos_end == HD44780_E2_POS_END && y > 1) {
    // A 40x4 line LCD has two 40x2 controllers.
    y -= 2;
  }

  if (y == 0) {
    hd44780_set_ddram_address(HD44780_LINE_1_DDRAM_ADDRESS_BASE + x);
  } else if (y == 1) {
    hd44780_set_ddram_address(HD44780_LINE_2_DDRAM_ADDRESS_BASE + x);
  } else if (y == 2) {
    hd44780_set_ddram_address(HD44780_LINE_1_DDRAM_ADDRESS_BASE + lcd_xsize + x);
  } else {
    hd44780_set_ddram_address(HD44780_LINE_2_DDRAM_ADDRESS_BASE + lcd_xsize + x);
  }

  lcd_pos = pos;
}

unsigned char hd44780_get_pos(void) {
  return lcd_pos;
}

void hd44780_set_ddram_address(unsigned char address) {
  hd44780_via_write_control(HD44780_INSTRUCTION_SET_DDRAM_ADDRESS | address, HD44780_VIA_CONTROL_E1 | HD44780_VIA_CONTROL_E2);
}

void hd44780_display_shift_left(void) {
  hd44780_via_write_control(HD44780_INSTRUCTION_CURSOR_DISPLAY | HD44780_CURSOR_DISPLAY_DISPLAY_SHIFT | HD44780_CURSOR_DISPLAY_LEFT, HD44780_VIA_CONTROL_E1 | HD44780_VIA_CONTROL_E2);
}

void hd44780_display_shift_right(void) {
  hd44780_via_write_control(HD44780_INSTRUCTION_CURSOR_DISPLAY | HD44780_CURSOR_DISPLAY_DISPLAY_SHIFT | HD44780_CURSOR_DISPLAY_RIGHT, HD44780_VIA_CONTROL_E1 | HD44780_VIA_CONTROL_E2);
}

void hd44780_display_off(void) {
  hd44780_via_write_control(HD44780_INSTRUCTION_DISPLAY_CONTROL | HD44780_DISPLAY_CONTROL_OFF, HD44780_VIA_CONTROL_E1 | HD44780_VIA_CONTROL_E2);
}

void hd44780_display_on(void) {
  hd44780_via_write_control(HD44780_INSTRUCTION_DISPLAY_CONTROL | HD44780_DISPLAY_CONTROL_ON, HD44780_VIA_CONTROL_E1 | HD44780_VIA_CONTROL_E2);
}

void hd44780_contrast_decrement(void) {
}

void hd44780_contrast_increment(void) {
}

void hd44780_orientation_default(void) {
}

void hd44780_orientation_rotated(void) {
}

void hd44780_putchar(char c) {
  if (lcd_pos < HD44780_E1_POS_END) {
    hd44780_via_write_data(c, HD44780_VIA_CONTROL_E1);
  } else {
    hd44780_via_write_data(c, HD44780_VIA_CONTROL_E2);
  }

  ++lcd_pos;
  if (lcd_pos == lcd_pos_end) {
    lcd_pos = 0;
  }

  if (lcd_pos % lcd_xsize == 0) {
    hd44780_set_pos(lcd_pos);
  }
}

void hd44780_via_write_control_init_hi(unsigned char data, unsigned char via_enable_bits) {
  unsigned char data_hi_with_control = (data & 0xF0);

  *hd44780_via_port = data_hi_with_control;
  *hd44780_via_port = data_hi_with_control | via_enable_bits;
  *hd44780_via_port = data_hi_with_control;

  delay_ms(5);
}

void hd44780_via_write_control(unsigned char data, unsigned char via_enable_bits) {
  unsigned char data_hi = ((data & 0xF0)     );
  unsigned char data_lo = ((data & 0x0F) << 4);

  hd44780_busy_wait();

  *hd44780_via_port = data_hi;
  *hd44780_via_port = data_hi | via_enable_bits;
  *hd44780_via_port = data_hi;

  *hd44780_via_port = data_lo;
  *hd44780_via_port = data_lo | via_enable_bits;
  *hd44780_via_port = data_lo;
}

void hd44780_via_write_data(unsigned char data, unsigned char via_enable_bits) {
  unsigned char data_hi_with_control = ((data & 0xF0)     ) | HD44780_VIA_CONTROL_RS;
  unsigned char data_lo_with_control = ((data & 0x0F) << 4) | HD44780_VIA_CONTROL_RS;

  hd44780_busy_wait();

  *hd44780_via_port = data_hi_with_control;
  *hd44780_via_port = data_hi_with_control | via_enable_bits;
  *hd44780_via_port = data_hi_with_control;

  *hd44780_via_port = data_lo_with_control;
  *hd44780_via_port = data_lo_with_control | via_enable_bits;
  *hd44780_via_port = data_lo_with_control;
}

void hd44780_busy_wait(void) {
  char data;

  VIA_DATA_DIRECTION_SET_WRITE_MASK(hd44780_via_ddr, HD44780_VIA_CONTROL_MASK);
  do {
    data = hd44780_via_read_control(HD44780_VIA_CONTROL_E1 | HD44780_VIA_CONTROL_E2);
  } while (data & HD44780_DR_BUSY);
  VIA_DATA_DIRECTION_SET_WRITE(hd44780_via_ddr);
}

unsigned char hd44780_via_read_control(unsigned char via_enable_bits) {
  unsigned char data_hi;
  unsigned char data_lo;

  *hd44780_via_port = HD44780_VIA_CONTROL_R_WB;
  *hd44780_via_port = HD44780_VIA_CONTROL_R_WB | via_enable_bits;
  data_hi = *hd44780_via_port & HD44780_VIA_DATA_MASK;

  *hd44780_via_port = HD44780_VIA_CONTROL_R_WB;
  *hd44780_via_port = HD44780_VIA_CONTROL_R_WB | via_enable_bits;
  data_lo = *hd44780_via_port & HD44780_VIA_DATA_MASK;

  *hd44780_via_port = HD44780_VIA_CONTROL_R_WB;
  return data_hi | (data_lo >> 4);
}
