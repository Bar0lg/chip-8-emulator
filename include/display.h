#pragma once
#include <stdbool.h>

#define SIZE_PIXEL 10
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

#define RAY_TOP SIZE_PIXEL *SCREEN_HEIGHT
#define RAY_RIGHT SIZE_PIXEL *SCREEN_WIDTH

extern bool **screen_pixels;

void init_screen();
void free_screen();
void draw_screen();
