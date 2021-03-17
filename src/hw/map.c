#include "map.h"

#define HW_REGISTER(address) ((volatile unsigned char*) address)

#define HW_VIA_B      HW_REGISTER(0x6000)
#define HW_VIA_A      HW_REGISTER(0x6001)
#define HW_VIA_DDRB   HW_REGISTER(0x6002)
#define HW_VIA_DDRA   HW_REGISTER(0x6003)
#define HW_VIA_T1C_L  HW_REGISTER(0x6004)
#define HW_VIA_T1C_H  HW_REGISTER(0x6005)
#define HW_VIA_T1L_L  HW_REGISTER(0x6006)
#define HW_VIA_T1L_H  HW_REGISTER(0x6007)
#define HW_VIA_ACR    HW_REGISTER(0x600B)
#define HW_VIA_IFR    HW_REGISTER(0x600D)
#define HW_VIA_IER    HW_REGISTER(0x600E)

#define HW_VIA_ACR_T1_CONTINUOUS_INTERRUPTS 0x40

#define HW_VIA_IFR_T1           0x40

#define HW_VIA_IER_RESET        0x00
#define HW_VIA_IER_SET          0x80
#define HW_VIA_IER_T1           0x40

#define HW_BUTTON_REGISTER          HW_VIA_A
#define HW_BUTTON_DDR               HW_VIA_DDRA
#define HW_BUTTON_MASK              (HW_BUTTON_0 | HW_BUTTON_1 | HW_BUTTON_2 | HW_BUTTON_3 | HW_BUTTON_4 | HW_BUTTON_5 | HW_BUTTON_6)

#define HW_LCD_REGISTER             HW_VIA_B
#define HW_LCD_DDR                  HW_VIA_DDRB
#define HW_LCD_CONTROL_MASK         0x0F
#define HW_LCD_DATA_MASK            0xF0

unsigned long hw_cpu_get_frequency(void) {
  return 2000000UL;
}

void hw_timer_fixed_rate_start(unsigned int frequency) {
  *HW_VIA_ACR = HW_VIA_ACR_T1_CONTINUOUS_INTERRUPTS;
  *HW_VIA_IER = HW_VIA_IER_SET | HW_VIA_IER_T1;
  *HW_VIA_T1C_L = frequency;
  *HW_VIA_T1C_H = (frequency >> 8);
}

unsigned char hw_timer_fixed_rate_interrupt_reset(void) {
  unsigned char reset = *HW_VIA_IFR & HW_VIA_IFR_T1;
  *HW_VIA_IFR |= HW_VIA_IFR_T1;
  return reset;
}

void hw_buzzer_direction_set_write(void) {
  // Note: the buzzer signal is the complement of the button signals.
  hw_button_direction_set_read();
}

void hw_buzzer_set(void) {
  *HW_BUTTON_REGISTER = HW_BUZZER;
}

void hw_buzzer_reset(void) {
  *HW_BUTTON_REGISTER = 0;
}

void hw_button_direction_set_read(void) {
  *HW_BUTTON_DDR = ~HW_BUTTON_MASK;
}

unsigned char hw_button_read(void) {
  return *HW_BUTTON_REGISTER & HW_BUTTON_MASK;
}

void hw_lcd_control_direction_set_write(void) {
  *HW_LCD_DDR = HW_LCD_CONTROL_MASK;
}

void hw_lcd_data_direction_set_write(void) {
  *HW_LCD_DDR = HW_LCD_CONTROL_MASK | HW_LCD_DATA_MASK;
}

void hw_lcd_data_direction_set_read(void) {
  *HW_LCD_DDR = HW_LCD_CONTROL_MASK;
}

void hw_lcd_write_8bit(unsigned char data, unsigned char rs, unsigned char e) {
  unsigned char data_hi_with_control = (data & 0xF0) | rs;

  *HW_LCD_REGISTER = data_hi_with_control;
  *HW_LCD_REGISTER = data_hi_with_control | e;
  *HW_LCD_REGISTER = data_hi_with_control;
}

void hw_lcd_write_4bit(unsigned char data, unsigned char rs, unsigned char e) {
  unsigned char data_hi_with_control = ((data & 0xF0)     ) | rs;
  unsigned char data_lo_with_control = ((data & 0x0F) << 4) | rs;

  *HW_LCD_REGISTER = data_hi_with_control;
  *HW_LCD_REGISTER = data_hi_with_control | e;
  *HW_LCD_REGISTER = data_hi_with_control;

  *HW_LCD_REGISTER = data_lo_with_control;
  *HW_LCD_REGISTER = data_lo_with_control | e;
  *HW_LCD_REGISTER = data_lo_with_control;
}

unsigned char hw_lcd_read_4bit(unsigned char rs, unsigned char e) {
  unsigned char data_hi;
  unsigned char data_lo;

  unsigned char control = HW_LCD_CONTROL_R_WB | rs;

  *HW_LCD_REGISTER = control;
  *HW_LCD_REGISTER = control | e;
  data_hi = *HW_LCD_REGISTER & HW_LCD_DATA_MASK;

  *HW_LCD_REGISTER = control;
  *HW_LCD_REGISTER = control | e;
  data_lo = *HW_LCD_REGISTER & HW_LCD_DATA_MASK;

  *HW_LCD_REGISTER = control;
  return data_hi | (data_lo >> 4);
}
