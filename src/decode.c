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
  case 2:
    *code = INS_CALL;
    *nnn = opcode & 0x0FFF;
    break;
  case 3:
    *code = INS_SEI;
    *x = (opcode & 0x0F00) >> 8;
    *kk = opcode & 0x00FF;
    break;
  case 4:
    *code = INS_SNEI;
    *x = (opcode & 0x0F00) >> 8;
    *kk = opcode & 0x00FF;
    break;
  case 5:
    *code = INS_SE;
    *x = (opcode & 0x0F00) >> 8;
    *y = (opcode & 0x00F0) >> 4;
    break;
  case 6:
    *code = INS_LDX;
    *x = (opcode & 0x0F00) >> 8;
    *kk = opcode & 0x00FF;
    break;
  case 7:
    *code = INS_ADDI;
    *x = (opcode & 0x0F00) >> 8;
    *kk = opcode & 0x00FF;
    break;
  case 8:
    uint16_t sec_code = opcode & 0x000F;
    *x = (opcode & 0x0F00) >> 8;
    *y = (opcode & 0x00F0) >> 4;
    switch (sec_code) {
    case 0:
      *code = INS_LDX;
      break;
    case 1:
      *code = INS_OR;
      break;
    case 2:
      *code = INS_AND;
      break;
    case 3:
      *code = INS_XOR;
      break;
    case 4:
      *code = INS_ADD;
      break;
    case 5:
      *code = INS_SUB;
      break;
    case 6:
      *code = INS_SHR;
      break;
    case 7:
      *code = INS_SUBN;
      break;
    case 8:
      *code = INS_SHL;
      break;
    default:
      break;
    }
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
