#pragma once

#include "../../../inc/MarlinConfig.h"

// Write buffer data to the SRAM or Flash
//  mem: 0x5A=32KB SRAM, 0xA5=16KB Flash
//  addr: start address
//  length: Bytes to write
//  data: address of the buffer with data
void dwinWriteToMem(uint8_t mem, uint16_t addr, uint16_t length, uint8_t *data) {
  const uint8_t max_size = 128;
  uint16_t pending = length;
  uint16_t to_send;
  uint16_t indx;
  uint8_t block = 0;

  while (pending > 0) {
    indx = block * max_size;
    to_send = _MIN(pending, max_size);
    size_t i = 0;
    dwinByte(i, 0x31);
    dwinByte(i, mem);
    dwinWord(i, addr + indx); // start address of the data block
    ++i;
    for (uint8_t j = 0; j < i; ++j) { LCD_SERIAL.write(dwinSendBuf[j]); delayMicroseconds(1); }  // Buf header
    for (uint16_t j = indx; j <= indx + to_send - 1; j++) LCD_SERIAL.write(*(data + j)); delayMicroseconds(1);  // write block of data
    for (uint8_t j = 0; j < 4; ++j) { LCD_SERIAL.write(dwinBufTail[j]); delayMicroseconds(1); }
    block++;
    pending -= to_send;
  }
}

inline void dwinByte(size_t &i, const uint16_t bval) {
  dwinSendBuf[++i] = bval;
}

inline void dwinWord(size_t &i, const uint16_t wval) {
  dwinSendBuf[++i] = wval >> 8;
  dwinSendBuf[++i] = wval & 0xFF;
}

inline void dwinLong(size_t &i, const uint32_t lval) {
  dwinSendBuf[++i] = (lval >> 24) & 0xFF;
  dwinSendBuf[++i] = (lval >> 16) & 0xFF;
  dwinSendBuf[++i] = (lval >>  8) & 0xFF;
  dwinSendBuf[++i] = lval & 0xFF;
}
