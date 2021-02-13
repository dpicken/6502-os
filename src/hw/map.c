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

#define HW_BUTTON_MASK              (HW_BUTTON_0 | HW_BUTTON_1 | HW_BUTTON_2 | HW_BUTTON_3 | HW_BUTTON_4)

#define HW_LCD_CONTROL_REGISTER     HW_VIA_A
#define HW_LCD_CONTROL_DDR          HW_VIA_DDRA
#define HW_LCD_CONTROL_MASK         (HW_LCD_CONTROL_BIT_E | HW_LCD_CONTROL_BIT_RW | HW_LCD_CONTROL_BIT_RS)

#define HW_LCD_DATA_REGISTER        HW_VIA_B
#define HW_LCD_DATA_DDR             HW_VIA_DDRB
#define HW_LCD_DATA_MASK            0xF0

unsigned long hw_cpu_get_frequency(void) {
  return 1000000UL;
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

void hw_via_ddra_direction_set(void) {
  *HW_VIA_DDRA = HW_LCD_CONTROL_MASK & ~HW_BUTTON_MASK;
}

void hw_button_direction_set_read(void) {
  hw_via_ddra_direction_set();
}

unsigned char hw_button_get(void) {
  return *HW_VIA_A & HW_BUTTON_MASK;
}

void hw_lcd_control_direction_set_write(void) {
  hw_via_ddra_direction_set();
}

void hw_lcd_control_set(unsigned char hw_lcd_control_bits) {
  *HW_VIA_A = hw_lcd_control_bits & HW_LCD_CONTROL_MASK;
}

void hw_lcd_data_direction_set_write(void) {
  *HW_VIA_DDRB = HW_LCD_DATA_MASK;
}

void hw_lcd_data_direction_set_read(void) {
  *HW_VIA_DDRB = 0x00;
}

void hw_lcd_data_write(unsigned char bits) {
  *HW_VIA_B = bits;
}

unsigned char hw_lcd_data_read(void) {
  return *HW_VIA_B & HW_LCD_DATA_MASK;
}
