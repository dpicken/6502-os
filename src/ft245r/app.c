#include "app.h"

#include "ft245r.h"

#include "controller/controller.h"
#include "display/display.h"
#include "io/io.h"
#include "timer/timer.h"

#include <stdio.h>
#include <stdlib.h>

static unsigned char echo_to_display_disabled;
static unsigned char echo_to_peer_disabled;

void ft245r_app_enter(void) {
  if (!ft245r_is_initialized()) {
    fprintf(stderr, "ft245r: not initialized\n");
    return;
  }

  controller_button_set_demuxed_depressed_handler(ft245r_app_rx, controller_button_a);
  controller_button_set_demuxed_depressed_handler(ft245r_app_tx, controller_button_b);
  controller_button_set_demuxed_depressed_handler(ft245r_app_echo_enter, controller_button_c);
  controller_button_set_demuxed_depressed_handler(ft245r_app_toggle_echo_to_display, controller_button_right);
  controller_button_set_demuxed_depressed_handler(ft245r_app_toggle_echo_to_peer, controller_button_up);
  controller_button_set_demuxed_depressed_handler(display_clear, controller_button_down);
}

void ft245r_app_rx(void) {
  unsigned char rx_ready = ft245r_rx_ready();

  timer_clear_all_non_special();

  fprintf(vdu, "RX: %s: %c\n"
      , rx_ready ? "ready" : "empty"
      , rx_ready ? ft245r_rx() : '\0');
}

void ft245r_app_tx(void) {
  unsigned char tx_ready = ft245r_tx_ready();
  unsigned char tx = 'A' + (rand() % 26);

  timer_clear_all_non_special();

  fprintf(vdu, "TX: %s: %c\n"
      , tx_ready ? "ready" : "full"
      , tx_ready ? (ft245r_tx(tx), tx) : '\0');
}

void ft245r_app_toggle_echo_to_display(void) {
  echo_to_display_disabled = !echo_to_display_disabled;
  display_clear();
  fprintf(vdu, "echo to display: %u\n", !echo_to_display_disabled);
}

void ft245r_app_toggle_echo_to_peer(void) {
  echo_to_peer_disabled = !echo_to_peer_disabled;
  display_clear();
  fprintf(vdu, "echo to peer: %u\n", !echo_to_peer_disabled);
}

void ft245r_app_echo_enter(void) {
  timer_clear_all_non_special();
  timer_add_fixed_rate(ft245r_app_echo, 1);
}

void ft245r_app_echo(void) {
  unsigned char c;

  while (ft245r_rx_ready()) {
    c = ft245r_rx();
    if (!echo_to_display_disabled) {
      fprintf(vdu, "%c", (char)c);
    }

    if (!echo_to_peer_disabled) {
      while (!ft245r_tx_ready());
      ft245r_tx(c);
    }
  }
}
