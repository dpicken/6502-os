#ifndef configure_app_h
#define configure_app_h

#include "ui/menu.h"

extern ui_menu configure_menu;

void configure_app_display_16x2(void);
void configure_app_display_20x4(void);
void configure_app_display_40x4(void);
void configure_app_display_contrast_increment(void);
void configure_app_display_contrast_decrement(void);
void configure_app_display_orientation_default(void);
void configure_app_display_orientation_rotated(void);
void configure_app_exit(void);

#endif // ifndef configure_app_h
