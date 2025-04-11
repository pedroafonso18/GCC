#include "cpu.h"

uint16_t get_16bit(uint8_t cpu1, uint8_t cpu2) {
    return(cpu1 << 8) | cpu2;
}

void set_16bit(uint16_t value, uint8_t* cpu1, uint8_t* cpu2) {
    *cpu1 = (value >> 8) & 0xFF;
    *cpu2 = value & 0xFF;
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

void execute_instructions(CPU* cpu, uint8_t* memory) {
    uint8_t opcode = memory[cpu->pc++];

    switch(opcode) {
        case 0x00: //NOP
            break;
        case 0x01: //LD BC, d16
            cpu->c = memory[cpu->pc++];
            cpu->b = memory[cpu->pc++];
            break;
        case 0x02: //LD [BC], A
            memory[get_16bit(cpu->b, cpu->c)] = cpu->a;
            break;
        case 0x03: //INC BC
            uint16_t bc = get_16bit(cpu->b, cpu->c);
            bc +=1;
            set_16bit(bc, &cpu->b, &cpu->c);
            break;
        case 0x04: //INC B
            cpu->b++;
            break;
        case 0x05: //DEC B
            cpu->b--;
            break;
        case 0x06: //LD B, d8
            cpu->b = memory[cpu->pc++];
            break;
        case 0x07: //RLCA
            uint8_t carry = (cpu->a & 0x80) >> 7;
            cpu->a = (cpu->a << 1) | carry;
            cpu->flag &= ~(FLAG_N | FLAG_H);
            cpu->flag = (cpu->flag & ~FLAG_C) | (carry ? FLAG_C : 0);
            break;
        case 0x08: //LD [a16], SP
            uint16_t address = memory[cpu->pc] | (memory[cpu->pc + 1] << 8);
            memory[address] = cpu->sp & 0xFF;
            memory[address + 1] = (cpu->sp >> 8) & 0xFF;
            cpu->pc += 2;
            break;
        case 0x09: //ADD HL, BC
            uint16_t hl = get_16bit(cpu->h, cpu->l);
            uint16_t bc = get_16bit(cpu->b, cpu->c);
            uint32_t result = hl + bc;

            if (((hl & 0x0FFF) + (bc & 0x0FFF)) > 0x0FFF)
                set_flag(cpu, FLAG_H);
            else {
                clear_flag(cpu, FLAG_H);
            }

            if (result > 0xFFFF) {
                set_flag(cpu, FLAG_C);
            }
            else {
                clear_flag(cpu, FLAG_C);
            }

            clear_flag(cpu, FLAG_N);

            set_16bit((uint16_t)result, &cpu->h, &cpu->l);
            break;
        case 0x0A: //LD A, [BC]
            cpu->a = memory[get_16bit(cpu->b, cpu->c)];
            break;
        case 0x0B: //DEC BC
            uint16_t bc = get_16bit(cpu->b, cpu->c);
            bc--;
            set_16bit(bc, &cpu->b, &cpu->c);
            set_flag(cpu, FLAG_N);
            break;
            case 0x0C: // INC C
            {
                if ((cpu->c & 0x0F) + 1 > 0x0F)
                    set_flag(cpu, FLAG_H);
                else
                    clear_flag(cpu, FLAG_H);
            
                cpu->c++;
            
                if (cpu->c == 0)
                    set_flag(cpu, FLAG_Z);
                else
                    clear_flag(cpu, FLAG_Z);
            
                clear_flag(cpu, FLAG_N);
                break;
            }            
        case 0x0D: //DEC C
            uint8_t result = cpu->c - 1;
            if (result == 0) {
                set_flag(cpu, FLAG_Z);
            } else {
                clear_flag(cpu, FLAG_Z);
            }
            set_flag(cpu, FLAG_N);

            if ((cpu->c & 0x0F) == 0){
                set_flag(cpu, FLAG_H);
            }
            else {
                clear_flag(cpu, FLAG_H);
            }
            cpu->c = result;
            break;
        case 0x0E: //LD C, d8
            cpu->c = memory[cpu->pc++];
            break;
        case 0x0F: //RRCA
            uint8_t carry = cpu->a & 0x80;
            cpu->a = (cpu->a >> 1) | (carry << 7);
            cpu->flag &= ~(FLAG_N | FLAG_H);
            cpu->flag = (cpu->flag & ~FLAG_C) | (carry ? FLAG_C : 0);
            break;
        case 0x11: //LD DE, d16
            cpu->e = memory[cpu->pc++];
            cpu->d = memory[cpu->pc++];
            break;
        case 0x12: //LD [DE], A
            memory[get_16bit(cpu->d, cpu->e)] = cpu->a;
            break;
        case 0x16: //LD D, d8
            cpu->d = memory[cpu->pc++];
            break;
        case 0x1A: //LD A, [DE]
            cpu->a = memory[get_16bit(cpu->d, cpu->e)];
            break;
        case 0x1E: //LD E, d8
            cpu->e = memory[cpu->pc++];
            break;
        case 0x21: //LD HL, d16
            cpu->l = memory[cpu->pc++];
            cpu->h = memory[cpu->pc++];
            break;
        case 0x22: //LD [HL+], A
            memory[get_16bit(cpu->h, cpu->l)] = cpu->a;
            set_16bit(get_16bit(cpu->h, cpu->l) + 1, &cpu->h, &cpu->l);
            break;
        case 0x26: //LD H, d8
            cpu->h = memory[cpu->pc++];
            break;
        case 0x2A: //LD A, [HL+]
            cpu->a = memory[get_16bit(cpu->h, cpu->l)];
            set_16bit(get_16bit(cpu->h, cpu->l) + 1, &cpu->h, &cpu->l);
            break;
        case 0x2E: //LD L, d8
            cpu->l = memory[cpu->pc++];
            break;
        case 0x31: //LD SP, d16
            cpu->sp = memory[cpu->pc] | (memory[cpu->pc + 1] << 8);
            cpu->pc += 2;
            break;
        case 0x32: //LD [HL-], A
            memory[get_16bit(cpu->h, cpu->l)] = cpu->a;
            set_16bit(get_16bit(cpu->h, cpu->l) - 1, &cpu->h, &cpu->l);
            break;
        case 0x3A: //LD A, [HL-]
            cpu->a = memory[get_16bit(cpu->h, cpu->l)];
            set_16bit(get_16bit(cpu->h, cpu->l) - 1, &cpu->h, &cpu->l);
            break;
        case 0x3E: //LD A, d8
            cpu->a = memory[cpu->pc++];
            break;
        case 0x40: //LD B, B
            break;
        case 0x41: //LD B, C
            cpu->b = cpu->c;
            break;
        case 0x42: //LD B, D
            cpu->b = cpu->d;
            break;
        case 0x43: //LD B, E
            cpu->b = cpu->e;
            break;
        case 0x44: //LD B, H
            cpu->b = cpu->h;
            break;
        case 0x45: //LD B, L
            cpu->b = cpu->l;
            break;
        case 0x46: //LD B, [HL]
            cpu->b = memory[get_16bit(cpu->h, cpu->l)];
            break;
        case 0x47: //LD B, A
            cpu->b = cpu->a;
            break;
        case 0x48: //LD C, B
            cpu->c = cpu->b;
            break;
        case 0x49: //LD C, C
            break;
        case 0x4A: //LD C, D
            cpu->c = cpu->d;
            break;
        case 0x4B: //LD C, E
            cpu->c = cpu->e;
            break;
        case 0x4C: //LD C, H
            cpu->c = cpu->h;
            break;
        case 0x4D: //LD C, L
            cpu->c = cpu->l;
            break;
        case 0x4E: //LD C, [HL]
            cpu->c = memory[get_16bit(cpu->h, cpu->l)];
            break;
        case 0x4F: //LD C, A
            cpu->c = cpu->a;
            break;
        case 0x50: //LD D, B
            cpu->d = cpu->b;
            break;
        case 0x51: //LD D, C
            cpu->d = cpu->c;
            break;
        case 0x52: //LD D, D
            break;
        case 0x53: //LD D, E
            cpu->d = cpu->e;
            break;
        case 0x54: //LD D, H
            cpu->d = cpu->h;
            break;
        case 0x55: //LD D, L
            cpu->d = cpu->l;
            break;
        case 0x56: //LD D, [HL]
            cpu->d = memory[get_16bit(cpu->h, cpu->l)];
            break;
        case 0x57: //LD D, A
            cpu->d = cpu->a;
            break;
        case 0x58: //LD E, B
            cpu->e = cpu->b;
            break;
        case 0x59: //LD E, C
            cpu->e = cpu->c;
            break;
        case 0x5A: //LD E, D
            cpu->e = cpu->d;
            break;
        case 0x5B: //LD E, E
            break;
        case 0x5C: //LD E, H
            cpu->e = cpu->h;
            break;
        case 0x5D: //LD E, L
            cpu->e = cpu->l;
            break;
        case 0x5E: //LD E, [HL]
            cpu->e = memory[get_16bit(cpu->h, cpu->l)];
            break;
        case 0x5F: //LD E, A
            cpu->e = cpu->a;
            break;
        case 0x60: //LD H, B
            cpu->h = cpu->b;
            break;
        case 0x61: //LD H, C
            cpu->h = cpu->c;
            break;
        case 0x62: //LD H, D
            cpu->h = cpu->d;
            break;
        case 0x63: //LD H, E
            cpu->h = cpu->e;
            break;
        case 0x64: //LD H, H
            break;
        case 0x65: //LD H, L
            cpu->h = cpu->l;
            break;
        case 0x66: //LD H, [HL]
            cpu->h = memory[get_16bit(cpu->h, cpu->l)];
            break;
        case 0x67: //LD H, A
            cpu->h = cpu->a;
            break;
        case 0x68: //LD L, B
            cpu->l = cpu->b;
            break;
        case 0x69: //LD L, C
            cpu->l = cpu->c;
            break;
        case 0x6A: //LD L, D
            cpu->l = cpu->d;
            break;
        case 0x6B: //LD L, E
            cpu->l = cpu->e;
            break;
        case 0x6C: //LD L, H
            cpu->l = cpu->h;
            break;
        case 0x6D: //LD L, L
            break;
        case 0x6E: //LD L, [HL]
            cpu->l = memory[get_16bit(cpu->h, cpu->l)];
            break;
        case 0x6F: //LD L, A
            cpu->l = cpu->a;
            break;
        case 0x70: //LD [HL], B
            memory[get_16bit(cpu->h, cpu->l)] = cpu->b;
            break;
        case 0x71: //LD [HL], C
            memory[get_16bit(cpu->h, cpu->l)] = cpu->c;
            break;
        case 0x72: //LD [HL], D
            memory[get_16bit(cpu->h, cpu->l)] = cpu->d;
            break;
        case 0x73: //LD [HL], E
            memory[get_16bit(cpu->h, cpu->l)] = cpu->e;
            break;
        case 0x74: //LD [HL], H
            memory[get_16bit(cpu->h, cpu->l)] = cpu->h;
            break;
        case 0x75: //LD [HL], L
            memory[get_16bit(cpu->h, cpu->l)] = cpu->l;
            break;
        case 0x76: //HALT
            break;
        case 0x77: //LD [HL], A
            memory[get_16bit(cpu->h, cpu->l)] = cpu->a;
            break;
        case 0x78: //LD A, B
            cpu->a = cpu->b;
            break;
        case 0x79: //LD A, C
            cpu->a = cpu->c;
            break;
        case 0x7A: //LD A, D
            cpu->a = cpu->d;
            break;
        case 0x7B: //LD A, E
            cpu->a = cpu->e;
            break;
        case 0x7C: //LD A, H
            cpu->a = cpu->h;
            break;
        case 0x7D: //LD A, L
            cpu->a = cpu->l;
            break;
        case 0x7E: //LD A, [HL]
            cpu->a = memory[get_16bit(cpu->h, cpu->l)];
            break;
        case 0x7F: //LD A, A
            break;
        case 0xE0: //LDH [a8], A
            memory[0xFF00 + memory[cpu->pc++]] = cpu->a;
            break;
        case 0xE2: //LD [C], A
            memory[0xFF00 + cpu->c] = cpu->a;
            break;
        case 0xEA: //LD [a16], A
            memory[memory[cpu->pc] | (memory[cpu->pc + 1] << 8)] = cpu->a;
            cpu->pc += 2;
            break;
        case 0xF0: //LDH A, [a8]
            cpu->a = memory[0xFF00 + memory[cpu->pc++]];
            break;
        case 0xF2: //LD A, [C]
            cpu->a = memory[0xFF00 + cpu->c];
            break;
        case 0xF9: //LD SP, HL
            cpu->sp = get_16bit(cpu->h, cpu->l);
            break;
        case 0xFA: //LD A, [a16]
            cpu->a = memory[memory[cpu->pc] | (memory[cpu->pc + 1] << 8)];
            cpu->pc += 2;
            break;
    }
}