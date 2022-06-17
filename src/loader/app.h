#ifndef loader_app_h
#define loader_app_h

#include "ui/menu.h"

extern ui_menu loader_menu;

void loader_app_load_sd(void);
void loader_app_load_sd_impl(void);
void loader_app_run(void);

#endif // ifndef loader_app_h
