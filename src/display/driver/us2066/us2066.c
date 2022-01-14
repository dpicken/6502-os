#include "us2066.h"

#include "us2066_constants.h"

#include "display/driver/hd44780/hd44780_constants.h"
#include "kernel/delay.h"
#include "via/via.h"

#define US2066_XSIZE_MAX 20
#define US2066_LINE_DDRAM_ADDRESS_OFFSET 0x20

#define US2066_CONTRAST_MIN 0
#define US2066_CONTRAST_MAX 255
#define US2066_CONTRAST_STEP 32

static hw_register us2066_via_port;
static hw_register us2066_via_ddr;

static unsigned char display_config_function_set_display_lines = US2066_FUNCTION_SET_DISPLAY_LINES_EVEN;
static unsigned char display_config_contrast = 0x7F;

static unsigned char display_xsize = US2066_XSIZE_MAX;
static unsigned char display_pos_end = 80;
static unsigned char display_pos;

const display_driver display_us2066 = {
  us2066_init,
  us2066_set_resolution,
  us2066_get_resolution_x,
  us2066_get_resolution_y,
  us2066_get_resolution_cell_count,
  us2066_clear,
  us2066_set_pos_home,
  us2066_set_pos,
  us2066_get_pos,
  us2066_display_shift_left,
  us2066_display_shift_right,
  us2066_display_off,
  us2066_display_on,
  us2066_contrast_decrement,
  us2066_contrast_increment,
  us2066_orientation_default,
  us2066_orientation_rotated,

  us2066_putchar
};

void us2066_init(hw_register via_port, hw_register via_ddr) {
  us2066_via_port = via_port;
  us2066_via_ddr = via_ddr;

  VIA_DATA_DIRECTION_SET_WRITE(us2066_via_ddr);

  us2066_via_write_control(HD44780_INSTRUCTION_ENTRY_MODE_SET
      | HD44780_ENTRY_MODE_SET_INCREMENT
      | HD44780_ENTRY_MODE_SET_SHIFT_DISPLAY_OFF);

  us2066_orientation_default();
  us2066_contrast_set();
  us2066_clear();

  us2066_via_write_control(HD44780_INSTRUCTION_DISPLAY_CONTROL
      | HD44780_DISPLAY_CONTROL_ON
      | HD44780_DISPLAY_CONTROL_CURSOR_OFF
      | HD44780_DISPLAY_CONTROL_CURSOR_BLINK_OFF);
}

void us2066_set_resolution(unsigned char x, unsigned char y) {
  display_xsize = x;
  display_pos_end = x * y;
  display_pos = 0;
}

unsigned char us2066_get_resolution_x(void) {
  return display_xsize;
}

unsigned char us2066_get_resolution_y(void) {
  return display_pos_end / display_xsize;
}

unsigned char us2066_get_resolution_cell_count(void) {
  return display_pos_end;
}

void us2066_clear(void) {
  us2066_via_write_control(HD44780_INSTRUCTION_CLEAR_DISPLAY);
  display_pos = 0;
}

void us2066_set_pos_home(void) {
  us2066_via_write_control(HD44780_INSTRUCTION_RETURN_HOME);
  display_pos = 0;
}

void us2066_set_pos(unsigned char pos) {
  unsigned char x = pos % display_xsize;
  unsigned char y = pos / display_xsize;

  us2066_set_ddram_address((US2066_LINE_DDRAM_ADDRESS_OFFSET * y) + x);
  display_pos = pos;
}

unsigned char us2066_get_pos(void) {
  return display_pos;
}

void us2066_set_ddram_address(unsigned char address) {
  us2066_via_write_control(HD44780_INSTRUCTION_SET_DDRAM_ADDRESS | address);
}

void us2066_display_shift_left(void) {
  us2066_via_write_control(HD44780_INSTRUCTION_CURSOR_DISPLAY | HD44780_CURSOR_DISPLAY_DISPLAY_SHIFT | HD44780_CURSOR_DISPLAY_LEFT);
}

void us2066_display_shift_right(void) {
  us2066_via_write_control(HD44780_INSTRUCTION_CURSOR_DISPLAY | HD44780_CURSOR_DISPLAY_DISPLAY_SHIFT | HD44780_CURSOR_DISPLAY_RIGHT);
}

void us2066_display_off(void) {
  us2066_via_write_control(HD44780_INSTRUCTION_DISPLAY_CONTROL | HD44780_DISPLAY_CONTROL_OFF);
}

void us2066_display_on(void) {
  us2066_via_write_control(HD44780_INSTRUCTION_DISPLAY_CONTROL | HD44780_DISPLAY_CONTROL_ON);
}

void us2066_contrast_decrement(void) {
  if (display_config_contrast > US2066_CONTRAST_STEP) {
    display_config_contrast -= US2066_CONTRAST_STEP;
  } else {
    display_config_contrast = US2066_CONTRAST_MIN;
  }
  us2066_contrast_set();
}

void us2066_contrast_increment(void) {
  if (display_config_contrast < US2066_CONTRAST_MAX - US2066_CONTRAST_STEP) {
    display_config_contrast += US2066_CONTRAST_STEP;
  } else {
    display_config_contrast = US2066_CONTRAST_MAX;
  }
  us2066_contrast_set();
}

void us2066_contrast_set(void) {
  us2066_via_write_control(HD44780_INSTRUCTION_FUNCTION_SET
      | display_config_function_set_display_lines
      | US2066_FUNCTION_SET_EXTENSION_RE_SET);
  us2066_via_write_control(US2066_INSTRUCTION_RE_OLED_CHARACTERIZATION
      | US2066_RE_OLED_CHARACTERIZATION_SD_SET);

  us2066_via_write_control(US2066_INSTRUCTION_RE_SD_SET_CONTRAST_1);
  us2066_via_write_control(US2066_INSTRUCTION_RE_SD_SET_CONTRAST_2
      | display_config_contrast);

  us2066_via_write_control(US2066_INSTRUCTION_RE_OLED_CHARACTERIZATION
      | US2066_RE_OLED_CHARACTERIZATION_SD_RESET);
  us2066_via_write_control(HD44780_INSTRUCTION_FUNCTION_SET
      | display_config_function_set_display_lines
      | US2066_FUNCTION_SET_EXTENSION_RE_RESET);
}

void us2066_orientation_default(void) {
  us2066_via_write_control(HD44780_INSTRUCTION_FUNCTION_SET
      | display_config_function_set_display_lines
      | US2066_FUNCTION_SET_EXTENSION_RE_SET);
  us2066_via_write_control(HD44780_INSTRUCTION_ENTRY_MODE_SET
      | US2066_RE_ENTRY_MODE_SET_BI_DIRECTION_COMMON_SET
      | US2066_RE_ENTRY_MODE_SET_BI_DIRECTION_SEGMENT_RESET);
  us2066_via_write_control(HD44780_INSTRUCTION_FUNCTION_SET
      | display_config_function_set_display_lines
      | US2066_FUNCTION_SET_EXTENSION_RE_RESET);
}

void us2066_orientation_rotated(void) {
  us2066_via_write_control(HD44780_INSTRUCTION_FUNCTION_SET
      | display_config_function_set_display_lines
      | US2066_FUNCTION_SET_EXTENSION_RE_SET);
  us2066_via_write_control(HD44780_INSTRUCTION_ENTRY_MODE_SET
      | US2066_RE_ENTRY_MODE_SET_BI_DIRECTION_COMMON_RESET
      | US2066_RE_ENTRY_MODE_SET_BI_DIRECTION_SEGMENT_SET);
  us2066_via_write_control(HD44780_INSTRUCTION_FUNCTION_SET
      | display_config_function_set_display_lines
      | US2066_FUNCTION_SET_EXTENSION_RE_RESET);
}

void us2066_putchar(char c) {
  us2066_via_write_data(c);

  ++display_pos;
  if (display_pos == display_pos_end) {
    display_pos = 0;
  }

  if (display_pos % display_xsize == 0) {
    us2066_set_pos(display_pos);
  }
}

void us2066_via_write_control(unsigned char data) {
  unsigned char data_hi = ((data & 0xF0)     );
  unsigned char data_lo = ((data & 0x0F) << 4);

  us2066_busy_wait();

  *us2066_via_port = data_hi;
  *us2066_via_port = data_hi | HD44780_VIA_CONTROL_E1;
  *us2066_via_port = data_hi;

  *us2066_via_port = data_lo;
  *us2066_via_port = data_lo | HD44780_VIA_CONTROL_E1;
  *us2066_via_port = data_lo;
}

void us2066_via_write_data(unsigned char data) {
  unsigned char data_hi_with_control = ((data & 0xF0)     ) | HD44780_VIA_CONTROL_RS;
  unsigned char data_lo_with_control = ((data & 0x0F) << 4) | HD44780_VIA_CONTROL_RS;

  us2066_busy_wait();

  *us2066_via_port = data_hi_with_control;
  *us2066_via_port = data_hi_with_control | HD44780_VIA_CONTROL_E1;
  *us2066_via_port = data_hi_with_control;

  *us2066_via_port = data_lo_with_control;
  *us2066_via_port = data_lo_with_control | HD44780_VIA_CONTROL_E1;
  *us2066_via_port = data_lo_with_control;
}

void us2066_busy_wait(void) {
  char data;

  VIA_DATA_DIRECTION_SET_WRITE_MASK(us2066_via_ddr, HD44780_VIA_CONTROL_MASK);
  do {
    data = us2066_via_read_control();
  } while (data & HD44780_DR_BUSY);
  VIA_DATA_DIRECTION_SET_WRITE(us2066_via_ddr);
}

unsigned char us2066_via_read_control(void) {
  unsigned char data_hi;
  unsigned char data_lo;

  *us2066_via_port = HD44780_VIA_CONTROL_R_WB;
  *us2066_via_port = HD44780_VIA_CONTROL_R_WB | HD44780_VIA_CONTROL_E1;
  data_hi = *us2066_via_port & 0xF0;

  *us2066_via_port = HD44780_VIA_CONTROL_R_WB;
  *us2066_via_port = HD44780_VIA_CONTROL_R_WB | HD44780_VIA_CONTROL_E1;
  data_lo = *us2066_via_port & 0xF0;

  *us2066_via_port = HD44780_VIA_CONTROL_R_WB;
  return data_hi | (data_lo >> 4);
}
