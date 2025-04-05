#ifndef CPU_H
#define CPU_H

#include <stdint.h>

typedef struct {
    uint8_t a, b, c, d, e, h, l;
    uint16_t sp;
    uint16_t pc;
} CPU;

uint16_t get_bc(CPU* cpu);

void set_bc(CPU* cpu, uint16_t value);

#endif
