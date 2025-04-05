#include "cpu.h"

uint16_t get_bc(CPU* cpu) {
    return(cpu->b << 8) | cpu->c;
}

void set_bc(CPU* cpu, uint16_t value) {
    cpu->b = (value >> 8) & 0xFF;
    cpu->c = value & 0xFF;
}