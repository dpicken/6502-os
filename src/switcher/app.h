#ifndef switcher_app_h
#define switcher_app_h

#include "controller/controller.h"
#include "ui/menu.h"

typedef void (switcher_app_runnable)(void);

extern ui_menu switcher_menu;

void switcher_app_enter(void);

void switcher_app_enter_controller(void);
void switcher_app_enter_display(void);
void switcher_app_enter_ft245r(void);
void switcher_app_enter_log(void);
void switcher_app_enter_memtest(void);
void switcher_app_enter_panic(void);
void switcher_app_enter_speedtest(void);
void switcher_app_enter_scroll(void);
void switcher_app_enter_wisdom(void);
void switcher_app_enter_uptime(void);
void switcher_app_enter_kernel_reset(void);
void switcher_app_enter_kernel_shutdown(void);
void switcher_app_start(switcher_app_runnable* app);
void switcher_app_return(void);
void switcher_app_reset(void);
void switcher_app_button_released(controller_button_bitset buttons);

#endif // ifndef switcher_app_h
