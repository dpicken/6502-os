#include "crc.h"

unsigned int crc16(unsigned int crc, unsigned char data) {
  crc = (crc << 8) | (crc >> 8);
  crc ^= data;
  crc ^= (crc & 0x00FF) >> 4;
  crc ^= (crc << 12);
  crc ^= (crc & 0x00FF) << 5;
  return crc;
}
