#include "computer.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "instruction.h"

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
    return (word >> 12) & 0xF;
}

static Instruction decode(BOB16 *bob16) {
    // TODO: See if this decoding is a waste and can just use the opcode for the return
    bobWord word = bob16->ram.memory[bob16->cpu.programCounter++];
    if (bob16->cpu.programCounter >= RAM_MAX) {
        fprintf(stderr, "TOO FAR");
    }
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
            return INS_TRAP;
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
            exit(0);
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

#ifdef TEST

int testDecode(BOB16 *bob16) {
    bobWord allInstructions[16] = {
        0x0000, // NOP
        0x1000, // ADD
        0x2000, // AND
        0x3000, // NOT
        0x4000, // LD
        0x5000, // LDI
        0x6000, // LDR
        0x7000, // ST
        0x8000, // STI
        0x9000, // STR
        0xA000, // BR
        0xB000, // JMP
        0xC000, // JSR
        0xD000, // LEA
        0xE000, // RET
        0xF000, // TRAP
    };

    memcpy(bob16->ram.memory, allInstructions, 16 * sizeof(bobWord));

    if (decode(bob16) != INS_NOP)  { printf("NOP doesn't decode\n"); return 1; }
    if (decode(bob16) != INS_ADD)  { printf("ADD doesn't decode\n"); return 1; }
    if (decode(bob16) != INS_AND)  { printf("AND doesn't decode\n"); return 1; }
    if (decode(bob16) != INS_NOT)  { printf("NOT doesn't decode\n"); return 1; }
    if (decode(bob16) != INS_LD)   { printf("LD doesn't decode\n"); return 1; }
    if (decode(bob16) != INS_LDI)  { printf("LDI doesn't decode\n"); return 1; }
    if (decode(bob16) != INS_LDR)  { printf("LDR doesn't decode\n"); return 1; }
    if (decode(bob16) != INS_ST)   { printf("ST doesn't decode\n"); return 1; }
    if (decode(bob16) != INS_STI)  { printf("STI doesn't decode\n"); return 1; }
    if (decode(bob16) != INS_STR)  { printf("STR doesn't decode\n"); return 1; }
    if (decode(bob16) != INS_BR)   { printf("BR doesn't decode\n"); return 1; }
    if (decode(bob16) != INS_JMP)  { printf("JMP doesn't decode\n"); return 1; }
    if (decode(bob16) != INS_JSR)  { printf("JSR doesn't decode\n"); return 1; }
    if (decode(bob16) != INS_LEA)  { printf("LEA doesn't decode\n"); return 1; }
    if (decode(bob16) != INS_RET)  { printf("RET doesn't decode\n"); return 1; }
    if (decode(bob16) != INS_TRAP) { printf("TRAP doesn't decode\n"); return 1; }

    return 0;
}

int testNOPInstruction(BOB16 *bob16) {
    return 0;
}

int testADDInstruction(BOB16 *bob16) {
    //                     0b0001,mddd,zzzo,ooii

    // reg = reg + reg
    bob16->cpu.regFile[0] = 15;
    bob16->cpu.regFile[1] = 15;
    bob16->ram.memory[0] = 0b0001,0010,0000,0100;
    // execute(bob16, INS_ADD);
    return 0;
}

int testANDInstruction(BOB16 *bob16) {
    //                     0b0001,mddd,zzzo,ooii
    return 0;
}

int main() {
    BOB16 bob16;
    initBOB16(&bob16);

    int hadError = 0;

    if (testDecode(&bob16)) {
        fprintf(stderr, "Decoding doesn't work\n");
        hadError = 1;
    }

    initBOB16(&bob16);

    if (testNOPInstruction(&bob16)) {
        fprintf(stderr, "NOP instruction doesn't work\n");
        hadError = 1;
    }

    initBOB16(&bob16);

    if (testADDInstruction(&bob16)) {
        fprintf(stderr, "ADD instruction doesn't work\n");
        hadError = 1;
    }

    initBOB16(&bob16);

    if (testANDInstruction(&bob16)) {
        fprintf(stderr, "AND instruction doesn't work\n");
        hadError = 1;
    }

    initBOB16(&bob16);

    if (!hadError) {
        printf("No errors found\n");
    }

    return hadError;
}

#endif
