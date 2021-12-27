#include "app.h"

#include "ft245r.h"

#include "console/console.h"
#include "controller/controller.h"
#include "timer/timer.h"

#include <stdio.h>
#include <stdlib.h>

void ft245r_app_enter(void) {
  if (!ft245r_is_initialized()) {
    printf("Not initialized\n");
    return;
  }
  controller_button_set_demuxed_depressed_handler(ft245r_app_rx, controller_button_a);
  controller_button_set_demuxed_depressed_handler(ft245r_app_tx, controller_button_b);
  controller_button_set_demuxed_depressed_handler(ft245r_app_echo_enter, controller_button_c);
  controller_button_set_demuxed_depressed_handler(console_clear, controller_button_down);
}

void ft245r_app_rx(void) {
  unsigned char rx_ready = ft245r_rx_ready();

  timer_clear_all_non_special();

  printf("RX: %s: %c\n"
      , rx_ready ? "ready" : "empty"
      , rx_ready ? ft245r_rx() : '\0');
}

void ft245r_app_tx(void) {
  unsigned char tx_ready = ft245r_tx_ready();
  unsigned char tx = 'A' + (rand() % 26);

  timer_clear_all_non_special();

  printf("TX: %s: %c\n"
      , tx_ready ? "ready" : "full"
      , tx_ready ? (ft245r_tx(tx), tx) : '\0');
}

void ft245r_app_echo_enter(void) {
  timer_clear_all_non_special();
  timer_add_fixed_rate(ft245r_app_echo, 20);
}

void ft245r_app_echo(void) {
  unsigned char c;

  while (ft245r_rx_ready()) {
    c = ft245r_rx();
    printf("%c", (char)c);
    if (!ft245r_tx_ready()) {
      printf("\necho skipped\n");
      continue;
    }
    ft245r_tx(c);
  }
}
