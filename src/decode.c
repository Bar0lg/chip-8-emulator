#include "vm.h"
#include <stdint.h>
#include <stdio.h>

void decode(char *addr, int *code, uint16_t *nnn, uint16_t *n, uint16_t *x,
            uint16_t *y, uint16_t *kk) {
  // printf("%p\n", addr);
  uint16_t opcode = ((uint16_t)(*(addr)) << 8) | ((uint16_t)(*(addr + 1)));

  uint8_t ins = opcode >> 12;
  // printf("%d\n", ins);

  switch (ins) {
  case 0:
    if (opcode == 0x00E0) {
      *code = INS_CLEAR;
      return;
    }
    if (opcode == 0x00EE) {
      *code = INS_RET;
    }
    *code = INS_NOP;
    return;
  case 1:
    *code = INS_JMP;
    *nnn = opcode & 0x0FFF;
    break;
  case 6:
    *code = INS_LDX;
    *x = (opcode & 0x0F00) >> 8;
    *kk = opcode & 0x00FF;
    break;
  case 7:
    *code = INS_ADD;
    *x = (opcode & 0x0F00) >> 8;
    *kk = opcode & 0x00FF;
    break;
  case 0xA:
    *code = INS_LDI;
    *nnn = opcode & 0x0FFF;
    break;
  case 0xD:
    *code = INS_DRW;
    *x = (opcode & 0x0F00) >> 8;
    *y = (opcode & 0x00F0) >> 4;
    *n = opcode & 0x000F;
    break;
  }
}
