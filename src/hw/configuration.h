#ifndef hw_configuration_h
#define hw_configuration_h

/**
 * CPU configuration.
 */
#if HW_CONFIGURATION_SBC_V0
  #define HW_CPU_FREQUENCY 4000000UL
#elif HW_CONFIGURATION_SBC_V1
  #define HW_CPU_FREQUENCY 8000000UL
#elif HW_CONFIGURATION_SBC_V2
  #define HW_CPU_FREQUENCY 8000000UL
#elif HW_CONFIGURATION_SBC_V2_2
  #define HW_CPU_FREQUENCY 8000000UL
#else
  #error unsupported hardware platform
#endif

/**
 * VIA configuration.
 */
#if HW_CONFIGURATION_SBC_V0
  #define HW_VIA_COUNT 1
  #define HW_VIA1_ADDRESS 0x4000
  #define HW_SYSTEM_TIMER_VIA via1
#elif HW_CONFIGURATION_SBC_V1
  #define HW_VIA_COUNT 1
  #define HW_VIA1_ADDRESS 0x6000
  #define HW_SYSTEM_TIMER_VIA via1
#elif HW_CONFIGURATION_SBC_V2
  #define HW_VIA_COUNT 2
  #define HW_VIA2_ADDRESS 0x5000
  #define HW_VIA1_ADDRESS 0x6000
  #define HW_SYSTEM_TIMER_VIA via1
#elif HW_CONFIGURATION_SBC_V2_2
  #define HW_VIA_COUNT 3
  #define HW_VIA1_ADDRESS 0x7F00
  #define HW_VIA2_ADDRESS 0x7F40
  #define HW_VIA3_ADDRESS 0x7F80
  #define HW_SYSTEM_TIMER_VIA via3
#else
  #error unsupported hardware platfrom
#endif

/**
 * Default input/output configuration.
 */
#if HW_CONFIGURATION_SBC_V0
  #define HW_CONTROLLER_VIA_PORT &via1->ra, &via1->ddra
  #define HW_DISPLAY_VIA_PORT &via1->rb, &via1->ddrb
  #define HW_DISPLAY_DRIVER_INIT display_init_hd44780
  #define HW_DISPLAY_DRIVER_INIT_ALT display_init_us2066
  #define HW_FT245R_SUPPORT 0
#elif HW_CONFIGURATION_SBC_V1
  #define HW_CONTROLLER_VIA_PORT &via1->rb, &via1->ddrb
  #define HW_DISPLAY_VIA_PORT &via1->ra, &via1->ddra
  #define HW_DISPLAY_DRIVER_INIT display_init_us2066
  #define HW_DISPLAY_DRIVER_INIT_ALT display_init_hd44780
  #define HW_FT245R_SUPPORT 0
#elif HW_CONFIGURATION_SBC_V2
  #define HW_CONTROLLER_VIA_PORT &via1->rb, &via1->ddrb
  #define HW_DISPLAY_VIA_PORT &via1->ra, &via1->ddra
  #define HW_DISPLAY_DRIVER_INIT display_init_us2066
  #define HW_DISPLAY_DRIVER_INIT_ALT display_init_hd44780
  #define HW_FT245R_SUPPORT 1
  #define HW_FT245R_CONTROL_VIA_PORT &via2->rb, &via2->ddrb
  #define HW_FT245R_DATA_VIA_PORT &via2->ra, &via2->ddra
#elif HW_CONFIGURATION_SBC_V2_2
  #define HW_CONTROLLER_VIA_PORT &via1->rb, &via1->ddrb
  #define HW_DISPLAY_VIA_PORT &via1->ra, &via1->ddra
  #define HW_DISPLAY_DRIVER_INIT display_init_us2066
  #define HW_DISPLAY_DRIVER_INIT_ALT display_init_hd44780
  #define HW_FT245R_SUPPORT 1
  #define HW_FT245R_CONTROL_VIA_PORT &via3->rb, &via3->ddrb
  #define HW_FT245R_DATA_VIA_PORT &via3->ra, &via3->ddra
#else
  #error unsupported hardware platfrom
#endif

#endif // ifndef hw_configuration_h
