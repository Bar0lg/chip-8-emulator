#pragma once

#include <stdint.h>

void decode(char *addr, int *code, uint16_t *nnn, uint16_t *n, uint16_t *x,
            uint16_t *y, uint16_t *kk);
