#include "display.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "vm.h"

VM el_VM;

int main() {
  srand(time(NULL));
  SetTraceLogLevel(LOG_WARNING);
  init_screen();
  el_VM.MEM = malloc(NB_BYTES);
  InitWindow(SCREEN_WIDTH * SIZE_PIXEL, SCREEN_HEIGHT * SIZE_PIXEL, "Chip-8");
  SetTargetFPS(60);
  for (int i = 0; i < SCREEN_HEIGHT; i++) {
    for (int j = 0; j < SCREEN_WIDTH; j++) {
      el_VM.SCREEN[i][j] = rand() % 2;
    }
  }
  while (!WindowShouldClose()) {
    BeginDrawing();
    draw_screen();
    EndDrawing();
  }
  CloseWindow();
  free_screen();
  free(el_VM.MEM);
  return 0;
}
