#include "app.h"

#include "console/console.h"
#include "hw/configuration.h"
#include "lcd/lcd.h"
#include "switcher/app.h"
#include "via/via.h"

#include <stdio.h>

static const ui_menu_item configure_lcd_menu_items[] = {
#if HW_VIA_COUNT > 1
  UI_MENU_MAKE_ITEM("Exp 1a:", configure_app_lcd_exp_1a),
  UI_MENU_MAKE_ITEM("Exp 2a:", configure_app_lcd_exp_2a),
  UI_MENU_MAKE_ITEM("Exp 2b:", configure_app_lcd_exp_2b),
#endif
  UI_MENU_MAKE_ITEM("16x2", configure_app_lcd_16x2),
  UI_MENU_MAKE_ITEM("20x4", configure_app_lcd_20x4),
  UI_MENU_MAKE_ITEM("40x4", configure_app_lcd_40x4),
  UI_MENU_MAKE_ITEM("Contrast +", configure_app_lcd_contrast_increment),
  UI_MENU_MAKE_ITEM("Contrast -", configure_app_lcd_contrast_decrement),
  UI_MENU_MAKE_ITEM("Orient Def", configure_app_lcd_orientation_default),
  UI_MENU_MAKE_ITEM("Orient Rot", configure_app_lcd_orientation_rotated),
};

static ui_menu configure_lcd_menu = UI_MENU_MAKE_SUB_MENU(&configure_menu, configure_lcd_menu_items);

static const ui_menu_item configure_menu_items[] = {
  UI_MENU_MAKE_ITEM_WITH_SUB_MENU("LCD", &configure_lcd_menu)
};

ui_menu configure_menu = UI_MENU_MAKE_SUB_MENU(&switcher_menu, configure_menu_items);

#if HW_VIA_COUNT > 1
void configure_app_lcd_exp_1a(void) {
  HW_LCD_DRIVER_INIT(&via1->ra, &via1->ddra);
  ui_menu_enter(&configure_lcd_menu);
}

void configure_app_lcd_exp_2a(void) {
  HW_LCD_DRIVER_INIT(&via2->ra, &via2->ddra);
  ui_menu_enter(&configure_lcd_menu);
}

void configure_app_lcd_exp_2b(void) {
  HW_LCD_DRIVER_INIT(&via2->rb, &via2->ddrb);
  ui_menu_enter(&configure_lcd_menu);
}
#endif

void configure_app_lcd_16x2(void) {
  console_set_resolution(16, 2);
  ui_menu_enter(&configure_lcd_menu);
}

void configure_app_lcd_20x4(void) {
  console_set_resolution(20, 4);
  ui_menu_enter(&configure_lcd_menu);
}

void configure_app_lcd_40x4(void) {
  console_set_resolution(40, 4);
  ui_menu_enter(&configure_lcd_menu);
}

void configure_app_lcd_contrast_increment(void) {
  lcd->contrast_increment();
}

void configure_app_lcd_contrast_decrement(void) {
  lcd->contrast_decrement();
}

void configure_app_lcd_orientation_default(void) {
  lcd->orientation_default();
  ui_menu_enter(&configure_lcd_menu);
}

void configure_app_lcd_orientation_rotated(void) {
  lcd->orientation_rotated();
  ui_menu_enter(&configure_lcd_menu);
}
