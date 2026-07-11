#include "decode.h"
#include "display.h"
#include "vm.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

void clear_screen() {
  for (int i = 0; i < SCREEN_HEIGHT; i++) {
    for (int j = 0; j < SCREEN_WIDTH; j++) {
      el_VM.SCREEN[i][j] = false;
    }
  }
}

void jump(uint16_t to) { el_VM.PC = to; }

void set_x_im(uint16_t x, uint16_t val) { el_VM.REG[x] = val; }

void add_x(uint16_t x, uint16_t val) { el_VM.REG[x] += val; }

void set_i(uint16_t val) { el_VM.I = val; }

void draw(uint16_t x, uint16_t y, uint16_t n) {
  uint16_t I_cpy = el_VM.I;
  uint16_t X_coordinate = el_VM.REG[x] % SCREEN_WIDTH;
  uint16_t Y_coordinate = el_VM.REG[y] % SCREEN_HEIGHT;
  el_VM.REG[0xF] = 0;
  for (int i = 0; i < n; i++) {
    if (Y_coordinate >= SCREEN_HEIGHT) {
      break;
    }
    char spr = el_VM.MEM[I_cpy + i];
    uint16_t X_tmp = X_coordinate;
    for (int b = 0; b < 8; b++) {
      if (X_tmp >= SCREEN_WIDTH) {
        break;
      }
      bool bit = spr & (1 << (7 - b));
      if (bit && el_VM.SCREEN[Y_coordinate][X_tmp]) {
        el_VM.SCREEN[Y_coordinate][X_tmp] = false;
        el_VM.REG[0xF] = 1;
      }
      if (bit && !el_VM.SCREEN[Y_coordinate][X_tmp]) {
        el_VM.SCREEN[Y_coordinate][X_tmp] = true;
      }
      X_tmp++;
    }
    Y_coordinate++;
  }
}

void ins_call(uint16_t nnn) {
  el_VM.STACK[el_VM.SP] = el_VM.PC;
  el_VM.SP++;
  el_VM.PC = nnn;
}

void fn_sei(uint16_t x, uint16_t kk) {
  if (el_VM.REG[x] == kk) {
    el_VM.PC += 2;
  }
}
void fn_snei(uint16_t x, uint16_t kk) {
  if (el_VM.REG[x] != kk) {
    el_VM.PC += 2;
  }
}

void fn_se(uint16_t x, uint16_t y) {
  if (el_VM.REG[x] == el_VM.REG[y]) {
    el_VM.PC += 2;
  }
}

void fn_ldx(uint16_t x, uint16_t y) { el_VM.REG[x] = el_VM.REG[y]; }
void fn_or(uint16_t x, uint16_t y) {
  el_VM.REG[x] = el_VM.REG[y] | el_VM.REG[x];
}
void fn_and(uint16_t x, uint16_t y) {
  el_VM.REG[x] = el_VM.REG[y] & el_VM.REG[x];
}
void fn_xor(uint16_t x, uint16_t y) {
  el_VM.REG[x] = el_VM.REG[y] ^ el_VM.REG[x];
}
void fn_add(uint16_t x, uint16_t y) {
  uint16_t tmp = (uint16_t)el_VM.REG[y] + (uint16_t)el_VM.REG[x];
  if (tmp > 255) {
    el_VM.REG[0xf] = 1;
  } else {
    el_VM.REG[0xf] = 0;
  }
  el_VM.REG[x] = tmp & 0xffff;
}
void fn_sub(uint16_t x, uint16_t y) {
  uint8_t tmp = el_VM.REG[x] - el_VM.REG[y];
  if (el_VM.REG[x] > el_VM.REG[y]) {
    el_VM.REG[0xf] = 1;
  } else {
    el_VM.REG[0xf] = 0;
  }
  el_VM.REG[x] = tmp & 0xffff;
}
void fn_shr(uint16_t x, uint16_t y) {
  (void)(y);
  if (el_VM.REG[x] & 0x1) {
    el_VM.REG[0xf] = 1;
  } else {
    el_VM.REG[0xf] = 0;
  }
  el_VM.REG[x] = el_VM.REG[x] >> 1;
}
void fn_subn(uint16_t x, uint16_t y) {
  uint8_t tmp = el_VM.REG[y] - el_VM.REG[x];
  (void)(y);
  if (el_VM.REG[y] > el_VM.REG[x]) {
    el_VM.REG[0xf] = 1;
  } else {
    el_VM.REG[0xf] = 0;
  }
  el_VM.REG[x] = tmp & 0xffff;
}
void fn_shl(uint16_t x, uint16_t y) {
  (void)(y);
  if (el_VM.REG[x] & 0b10000000) {
    el_VM.REG[0xf] = 1;
  } else {
    el_VM.REG[0xf] = 0;
  }
  el_VM.REG[x] = el_VM.REG[x] << 1;
}

void decode_and_exe() {
  int code;
  uint16_t x, y, kk, nnn, n;
  decode(el_VM.MEM + el_VM.PC, &code, &nnn, &n, &x, &y, &kk);
  el_VM.PC += 2;
  switch (code) {
  case INS_CLEAR:
    clear_screen();
    break;
  case INS_JMP:
    jump(nnn);
    break;
  case INS_LDX:
    set_x_im(x, kk);
    break;
  case INS_ADDI:
    add_x(x, kk);
    break;
  case INS_LDI:
    set_i(nnn);
    break;
  case INS_DRW:
    draw(x, y, n);
    break;
  case INS_CALL:
    ins_call(nnn);
    break;
  case INS_SEI:
    fn_sei(x, kk);
    break;
  case INS_SNEI:
    fn_snei(x, kk);
    break;
  case INS_SE:
    fn_se(x, y);
    break;
  case INS_LDXI:
    fn_ldx(x, y);
    break;
  case INS_OR:
    fn_or(x, y);
    break;
  case INS_AND:
    fn_and(x, y);
    break;
  case INS_XOR:
    fn_xor(x, y);
    break;
  case INS_ADD:
    fn_add(x, y);
    break;
  case INS_SUB:
    fn_sub(x, y);
    break;
  case INS_SHR:
    fn_shr(x, y);
    break;
  case INS_SUBN:
    fn_subn(x, y);
    break;
  case INS_SHL:
    fn_shl(x, y);
    break;

  default:
    break;
  }
}
