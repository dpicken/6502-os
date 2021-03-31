#include "hex.h"

static const char* hex_chars = "0123456789ABCDEF";

char hex_get_hi_char(unsigned char byte) {
  return hex_chars[byte >> 4];
}

char hex_get_lo_char(unsigned char byte) {
  return hex_chars[byte & 0xF];
}
