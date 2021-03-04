#ifndef configure_app_h
#define configure_app_h

#include "ui/menu.h"

extern ui_menu configure_menu;

void configure_app_console_16x2(void);
void configure_app_console_20x4(void);
void configure_app_console_40x4(void);
void configure_app_exit(void);

#endif // ifndef configure_app_h
