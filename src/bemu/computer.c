#include "computer.h"

#include <stdbool.h>

static inline void initCPU(CPU *cpu) {
    memset(cpu->regFile, 0, REG_COUNT);
    cpu->programCounter = 0;
}

static inline void initRAM(RAM *ram) {
    memset(ram->memory, 0, RAM_MAX);
}

void initBOB16(BOB16 *bob16) {
    initCPU(&bob16->cpu);
    initRAM(&bob16->ram);
}

static int getOpcode(bobWord word) {
    (word >> 12) & 0xF;
}

static Instruction decode(BOB16 *bob16) {
    // TODO: See if this decoding is a waste and can just use the opcode for the return
    bobWord word = bob16->ram.memory[bob16->cpu.programCounter++];
    switch(getOpcode(word)) {
        case 0x0:
            return INS_NOP;
        case 0x1:
            return INS_ADD;
        case 0x2:
            return INS_AND;
        case 0x3:
            return INS_NOT;
        case 0x4:
            return INS_LD;
        case 0x5:
            return INS_LDI;
        case 0x6:
            return INS_LDR;
        case 0x7:
            return INS_ST;
        case 0x8:
            return INS_STI;
        case 0x9:
            return INS_STR;
        case 0xA:
            return INS_BR;
        case 0xB:
            return INS_JMP;
        case 0xC:
            return INS_JSR;
        case 0xD:
            return INS_LEA;
        case 0xE:
            return INS_RET;
        case 0xF:
            return INS_RET;
    }
}

static void execute(BOB16 *bob16, Instruction instruction) {
    switch (instruction) {
        case INS_NOP:
            return;
        case INS_ADD:
            return;
        case INS_AND:
            return;
        case INS_NOT:
            break;
        case INS_LD:
            return;
        case INS_LDI:
            return;
        case INS_LDR:
            return;
        case INS_ST:
            return;
        case INS_STI:
            return;
        case INS_STR:
            return;
        case INS_BR:
            return;
        case INS_JMP:
            return;
        case INS_JSR:
            return;
        case INS_LEA:
            return;
        case INS_RET:
            return;
        case INS_TRAP:
            return;
    }
}

static void clockCycle(BOB16 *bob16) {
    Instruction instruction = decode(bob16);
    execute(bob16, instruction);
}

void run(BOB16 *bob16) {
    while (true) {
        clockCycle(bob16);
    }
}
