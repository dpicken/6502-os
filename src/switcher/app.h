#ifndef switcher_app_h
#define switcher_app_h

#include "ui/menu.h"

extern ui_menu switcher_menu;

void switcher_app_enter(void);

void switcher_app_enter_button(void);
void switcher_app_enter_buzzer(void);
void switcher_app_enter_lcd(void);
void switcher_app_enter_log(void);
void switcher_app_enter_memtest(void);
void switcher_app_enter_scroll(void);
void switcher_app_enter_uptime(void);
void switcher_app_exit(void);
void switcher_app_reset(void);

#endif // ifndef switcher_app_h
