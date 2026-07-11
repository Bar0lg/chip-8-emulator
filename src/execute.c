#include "decode.h"
#include "display.h"
#include "vm.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

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
      printf("%d-%d\n", X_tmp, Y_coordinate);
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
  case INS_ADD:
    add_x(x, kk);
    break;
  case INS_LDI:
    set_i(nnn);
    break;
  case INS_DRW:
    draw(x, y, n);
    break;
  default:
    break;
  }
}
