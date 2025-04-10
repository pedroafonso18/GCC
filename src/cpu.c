#include "cpu.h"

uint16_t get_bc(CPU* cpu) {
    return(cpu->b << 8) | cpu->c;
}

uint16_t get_af(CPU* cpu) {
    return(cpu->a << 8) | cpu->flag;
}

uint16_t get_de(CPU* cpu) {
    return(cpu->d << 8) | cpu->e;
}

void set_bc(CPU* cpu, uint16_t value) {
    cpu->b = (value >> 8) & 0xFF;
    cpu->c = value & 0xFF;
}

void set_af(CPU* cpu, uint16_t value) {
    cpu->a = value >> 8;
    cpu->flag = value & 0xF0;
}

void set_de(CPU* cpu, uint16_t value) {
    cpu->d = (value >> 8) & 0xFF;
    cpu->e = value & 0xFF;
}

void set_flag(CPU* cpu, uint8_t flag) {
    cpu->flag |= flag;
}

void clear_flag(CPU* cpu, uint8_t flag) {
    cpu->flag &= ~flag;
}

bool is_flag_set(CPU* cpu, uint8_t flag) {
    return cpu->flag & flag;
}