#pragma once

#include <stdbool.h>
#include <stdint.h>

enum INS {
  INS_NOP,
  INS_CLEAR,
  INS_RET,
  INS_JMP,
  INS_CALL,
  INS_SEI,
  INS_SNEI,
  INS_SE,
  INS_LDXI,
  INS_LDX,
  INS_OR,
  INS_AND,
  INS_XOR,
  INS_ADD,
  INS_SUB,
  INS_SHR,
  INS_SUBN,
  INS_SHL,
  INS_SNE,
  INS_LDI,
  INS_JP0,
  INS_RND,
  INS_DRW,
  INS_SKPK,
  INS_SKPNK,
  INS_LDT,
  INS_LDST,
  INS_ADDI,
  INS_LDS,
  INS_LDB,
  INS_STM,
  INS_LDM
};

#define NB_BYTES 4096
#define START_PROG 512

#define TIMER_RATE 60
#define INS_PER_SECOND 700
typedef struct {
  uint8_t REG[16];
  bool **SCREEN;
  uint16_t I;
  uint8_t TIMER;
  uint8_t S_TIMER;
  uint16_t PC;
  uint16_t STACK[16];
  uint8_t SP;
  char *MEM;
} VM;

extern VM el_VM;
