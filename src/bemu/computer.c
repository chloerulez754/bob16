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
    bobWord NOP = 0x0000;

    bob16->ram.memory[0] = NOP;

    BOB16 ogBob16;
    memcpy(&ogBob16, bob16, sizeof(BOB16));

    ogBob16.cpu.programCounter++;

    clockCycle(bob16);

    if (memcmp(&ogBob16, bob16, sizeof(BOB16)) != 0) return 1;

    return 0;
}

int testADDInstruction(BOB16 *bob16) {
    // reg = reg + reg
    bob16->cpu.regFile[1] = 25;
    bob16->cpu.regFile[2] = 15;
    bob16->ram.memory[0] = 0x1012;
    clockCycle(bob16);
    if (bob16->cpu.regFile[0] != 40) {
        fprintf(stderr, "`reg = reg + reg` does not work\n");
        fprintf(stderr, "r0 should be 40, r0 is %d\n", bob16->cpu.regFile[0]);
        return 1;
    }

    // reg = reg + imm
    bob16->cpu.regFile[1] = 25;
    bob16->ram.memory[1] = 0x1415;
    clockCycle(bob16);
    if (bob16->cpu.regFile[0] != 30) {
        fprintf(stderr, "`reg = reg + imm` does not work\n");
        fprintf(stderr, "r0 should be 30, r0 is %d\n", bob16->cpu.regFile[0]);
        return 1;
    }

    // reg += reg
    bob16->cpu.regFile[0] = 25;
    bob16->cpu.regFile[1] = 15;
    bob16->ram.memory[2] = 0x1810;
    clockCycle(bob16);
    if (bob16->cpu.regFile[0] != 40) {
        fprintf(stderr, "`reg += reg` does not work\n");
        fprintf(stderr, "r0 should be 40, r0 is %d\n", bob16->cpu.regFile[0]);
        return 1;
    }

    // reg += imm
    bob16->cpu.regFile[0] = 25;
    bob16->ram.memory[3] = 0x1C05;
    clockCycle(bob16);
    if (bob16->cpu.regFile[0] != 30) {
        fprintf(stderr, "`reg += imm` does not work\n");
        fprintf(stderr, "r0 should be 30, r0 is %d\n", bob16->cpu.regFile[0]);
        return 1;
    }

    return 0;
}

int testANDInstruction(BOB16 *bob16) {
    // reg = reg & reg
    bob16->cpu.regFile[1] = 25;
    bob16->cpu.regFile[2] = 15;
    bob16->ram.memory[0] = 0x2012;
    clockCycle(bob16);
    if (bob16->cpu.regFile[0] != 9) {
        fprintf(stderr, "`reg = reg & reg` does not work\n");
        fprintf(stderr, "r0 should be 9, r0 is %d\n", bob16->cpu.regFile[0]);
        return 1;
    }

    // reg = reg & imm
    bob16->cpu.regFile[1] = 25;
    bob16->ram.memory[1] = 0x2415;
    clockCycle(bob16);
    if (bob16->cpu.regFile[0] != 1) {
        fprintf(stderr, "`reg = reg & imm` does not work\n");
        fprintf(stderr, "r0 should be 1, r0 is %d\n", bob16->cpu.regFile[0]);
        return 1;
    }

    // reg &= reg
    bob16->cpu.regFile[0] = 25;
    bob16->cpu.regFile[1] = 15;
    bob16->ram.memory[2] = 0x2810;
    clockCycle(bob16);
    if (bob16->cpu.regFile[0] != 9) {
        fprintf(stderr, "`reg &= reg` does not work\n");
        fprintf(stderr, "r0 should be 9, r0 is %d\n", bob16->cpu.regFile[0]);
        return 1;
    }

    // reg &= imm
    bob16->cpu.regFile[0] = 25;
    bob16->ram.memory[3] = 0x2C05;
    clockCycle(bob16);
    if (bob16->cpu.regFile[0] != 1) {
        fprintf(stderr, "`reg &= imm` does not work\n");
        fprintf(stderr, "r0 should be 1, r0 is %d\n", bob16->cpu.regFile[0]);
        return 1;
    }

    return 0;
}

int testNOTInstruction(BOB16 *bob16) {
    // reg = ~reg
    bob16->cpu.regFile[1] = 0xF0F0;
    bob16->ram.memory[0] = 0x3010;
    clockCycle(bob16);
    if (bob16->cpu.regFile[0] != 0x0F0F) {
        fprintf(stderr, "`reg = ~reg` does not work\n");
        fprintf(stderr, "r0 should be 0x0F0F, r0 is %X\n", bob16->cpu.regFile[0]);
        return 1;
    }

    // reg = ~imm
    bob16->ram.memory[0] = 0x3405;
    clockCycle(bob16);
    if (bob16->cpu.regFile[0] != 0xFFFA) {
        fprintf(stderr, "`reg = ~imm` does not work\n");
        fprintf(stderr, "r0 should be 0xFFFA, r0 is %X\n", bob16->cpu.regFile[0]);
        return 1;
    }

    // ~=reg
    bob16->cpu.regFile[0] = 0x0F0F;
    bob16->ram.memory[1] = 0x3800; // not r0
    clockCycle(bob16);
    if (bob16->cpu.regFile[0] != 0xF0F0) {
        fprintf(stderr, "`~=reg` does not work\n");
        fprintf(stderr, "r0 should be 0xF0F0, r0 is %d\n", bob16->cpu.regFile[0]);
        return 1;
    }
    return 0;
}

int testLDInstruction(BOB16 *bob16) {
    return 0;
}

int testLDIInstruction(BOB16 *bob16) {
    return 0;
}

int testLDRInstruction(BOB16 *bob16) {
    return 0;
}

int testSTInstruction(BOB16 *bob16) {
    return 0;
}

int testSTIInstruction(BOB16 *bob16) {
    return 0;
}

int testSTRInstruction(BOB16 *bob16) {
    return 0;
}

int testBRInstruction(BOB16 *bob16) {
    return 0;
}

int testJMPInstruction(BOB16 *bob16) {
    return 0;
}

int testJSRInstruction(BOB16 *bob16) {
    return 0;
}

int testRETInstruction(BOB16 *bob16) {
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

    if (testNOTInstruction(&bob16)) {
        fprintf(stderr, "NOT instruction doesn't work\n");
    }

    initBOB16(&bob16);

    // TODO: Make all the rest of the integration tests 

    if (testLDInstruction(&bob16)) {
        fprintf(stderr, "NOP instruction doesn't work\n");
        hadError = 1;
    }

    initBOB16(&bob16);

    if (testLDIInstruction(&bob16)) {
        fprintf(stderr, "ADD instruction doesn't work\n");
        hadError = 1;
    }

    initBOB16(&bob16);

    if (testLDRInstruction(&bob16)) {
        fprintf(stderr, "AND instruction doesn't work\n");
        hadError = 1;
    }

    initBOB16(&bob16);

    if (testSTInstruction(&bob16)) {
        fprintf(stderr, "NOT instruction doesn't work\n");
    }

    initBOB16(&bob16);

    if (testSTIInstruction(&bob16)) {
        fprintf(stderr, "NOP instruction doesn't work\n");
        hadError = 1;
    }

    initBOB16(&bob16);

    if (testSTRInstruction(&bob16)) {
        fprintf(stderr, "ADD instruction doesn't work\n");
        hadError = 1;
    }

    initBOB16(&bob16);

    if (testBRInstruction(&bob16)) {
        fprintf(stderr, "AND instruction doesn't work\n");
        hadError = 1;
    }

    initBOB16(&bob16);

    if (testJMPInstruction(&bob16)) {
        fprintf(stderr, "NOT instruction doesn't work\n");
    }

    initBOB16(&bob16);

    if (testJSRInstruction(&bob16)) {
        fprintf(stderr, "NOP instruction doesn't work\n");
        hadError = 1;
    }

    initBOB16(&bob16);

    if (testRETInstruction(&bob16)) {
        fprintf(stderr, "ADD instruction doesn't work\n");
        hadError = 1;
    }

    if (!hadError) {
        printf("No errors found\n");
    }

    return hadError;
}

#endif
