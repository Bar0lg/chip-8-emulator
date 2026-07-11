#include "display.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "execute.h"
#include "vm.h"
VM el_VM;

void load_file(char *filename) {
  printf("%s\n", filename);
  FILE *fd = fopen(filename, "r");
  char *addr = el_VM.MEM + START_PROG;
  while (!feof(fd)) {
    *addr = fgetc(fd);
    addr++;
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: chip8 <file>\n");
    return 1;
  }
  srand(time(NULL));
  SetTraceLogLevel(LOG_WARNING);
  init_screen();
  el_VM.MEM = malloc(NB_BYTES);
  load_file(argv[1]);
  el_VM.PC = START_PROG;
  el_VM.SP = 0;
  InitWindow(SCREEN_WIDTH * SIZE_PIXEL, SCREEN_HEIGHT * SIZE_PIXEL, "Chip-8");
  SetTargetFPS(TIMER_RATE);
  while (!WindowShouldClose()) {
    for (int i = 0; i < (INS_PER_SECOND / TIMER_RATE); i++) {
      decode_and_exe();
    }
    BeginDrawing();
    draw_screen();
    if (el_VM.TIMER > 0) {
      el_VM.TIMER--;
    }
    if (el_VM.S_TIMER > 0) {
      el_VM.S_TIMER--;
    }
    EndDrawing();
  }
  CloseWindow();
  free_screen();
  free(el_VM.MEM);
  return 0;
}
