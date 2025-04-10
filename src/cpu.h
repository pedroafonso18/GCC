#ifndef CPU_H
#define CPU_H

#define FLAG_Z (1 << 7)
#define FLAG_N (1 << 6)
#define FLAG_H (1 << 5)
#define FLAG_C (1 << 4)

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint8_t a, b, c, d, e, h, l, flag;
    uint16_t sp;
    uint16_t pc;
} CPU;

uint16_t get_bc(CPU *cpu);

uint16_t get_af(CPU *cpu);

uint16_t get_de(CPU *cpu);


void set_af(CPU *cpu, uint16_t value);

void set_de(CPU *cpu, uint16_t value);

void set_bc(CPU *cpu, uint16_t value);

void set_flag(CPU *cpu, uint8_t flag);

void clear_flag(CPU *cpu, uint8_t flag);

bool is_flag_set(CPU *cpu, uint8_t flag);

#endif
