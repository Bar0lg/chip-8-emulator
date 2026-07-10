#include "display.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
bool **screen_pixels = NULL;

void init_screen() {
  bool **res = malloc(SCREEN_HEIGHT * sizeof(bool *));
  for (int i = 0; i < SCREEN_HEIGHT; i++) {
    bool *tmp = calloc(SCREEN_WIDTH, sizeof(bool));
    res[i] = tmp;
  }
  screen_pixels = res;
}

void draw_screen() {
  for (int i = 0; i < SCREEN_HEIGHT; i++) {
    for (int j = 0; j < SCREEN_WIDTH; j++) {
      if (screen_pixels[i][j]) {
        DrawRectangle(j * SIZE_PIXEL, i * SIZE_PIXEL, SIZE_PIXEL, SIZE_PIXEL,
                      WHITE);
      } else {
        DrawRectangle(j * SIZE_PIXEL, i * SIZE_PIXEL, SIZE_PIXEL, SIZE_PIXEL,
                      BLACK);
      }
    }
  }
}

void free_screen() {
  for (int i = 0; i < SCREEN_HEIGHT; i++) {
    free(screen_pixels[i]);
  }
  free(screen_pixels);
}
