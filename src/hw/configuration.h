#ifndef hw_configuration_h
#define hw_configuration_h

//#define HW_CONFIGURATION_BREADBOARD 1
//#define HW_CONFIGURATION_SBC_V1 1
#define HW_CONFIGURATION_SBC_V2 1

/**
 * CPU configuration.
 */
#if HW_CONFIGURATION_BREADBOARD
  #define HW_CPU_FREQUENCY 6000000UL
#elif HW_CONFIGURATION_SBC_V1
  #define HW_CPU_FREQUENCY 8000000UL
#elif HW_CONFIGURATION_SBC_V2
  #define HW_CPU_FREQUENCY 8000000UL
#else
  #error unsupported hardware platform
#endif

/**
 * VIA configuration.
 */
#if HW_CONFIGURATION_BREADBOARD
  #define HW_VIA_COUNT 1
  #define HW_VIA1_ADDRESS 0x6000
#elif HW_CONFIGURATION_SBC_V1
  #define HW_VIA_COUNT 1
  #define HW_VIA1_ADDRESS 0x6000
#elif HW_CONFIGURATION_SBC_V2
  #define HW_VIA_COUNT 2
  #define HW_VIA1_ADDRESS 0x6000
  #define HW_VIA2_ADDRESS 0x5000
#else
  #error unsupported hardware platfrom
#endif

/**
 * Default input/output configuration.
 */
#if HW_CONFIGURATION_BREADBOARD
  #define HW_CONTROLLER_VIA1_PORT &via1->ra, &via1->ddra
  #define HW_DISPLAY_VIA1_PORT &via1->rb, &via1->ddrb
  #define HW_DISPLAY_DRIVER_INIT display_init_hd44780
#elif HW_CONFIGURATION_SBC_V1
  #define HW_CONTROLLER_VIA1_PORT &via1->rb, &via1->ddrb
  #define HW_DISPLAY_VIA1_PORT &via1->ra, &via1->ddra
  #define HW_DISPLAY_DRIVER_INIT display_init_us2066
#elif HW_CONFIGURATION_SBC_V2
  #define HW_CONTROLLER_VIA1_PORT &via1->rb, &via1->ddrb
  #define HW_DISPLAY_VIA1_PORT &via1->ra, &via1->ddra
  #define HW_DISPLAY_DRIVER_INIT display_init_us2066
  #define HW_FT245R_CONTROL_VIA2_PORT &via2->rb, &via2->ddrb
  #define HW_FT245R_DATA_VIA2_PORT &via2->ra, &via2->ddra
#else
  #error unsupported hardware platfrom
#endif

#endif // ifndef hw_configuration_h
