#include "computer.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "instruction.h"

static bobWord signExtend(bobWord word, size_t length) {
    size_t n = 16 - length;
    return (bobWord)(word << n) >> n;
}

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

static void addInstruction(BOB16 *bob16, bobWord word) {
    size_t dest = (word >> 7) & 0x7;
    size_t reg = (word >> 4) & 0x7;
    bobWord imm;

    switch ((word >> 10) & 3) {
        case 0:
            // dest = reg + reg1
            size_t reg1 = (word >> 1) & 0x7;
            bob16->cpu.regFile[dest] = bob16->cpu.regFile[reg] + bob16->cpu.regFile[reg1];
            break;
        case 1:
            // dest = reg + imm
            imm = signExtend(word & 0xF, 4);
            bob16->cpu.regFile[dest] = bob16->cpu.regFile[reg] + imm;
            break;
        case 2:
            // dest += reg
            bob16->cpu.regFile[dest] += bob16->cpu.regFile[reg];
            break;
        case 3:
            // dest += imm
            imm = signExtend(word & 0x7F, 7);
            bob16->cpu.regFile[dest] += imm;
            break;
    }
}

static void andInstruction(BOB16 *bob16, bobWord word) {
    size_t dest = (word >> 7) & 0x7;
    size_t reg = (word >> 4) & 0x7;
    bobWord imm;

    switch ((word >> 10) & 3) {
        case 0:
            // dest = reg & reg1
            size_t reg1 = (word >> 1) & 0x7;
            bob16->cpu.regFile[dest] = bob16->cpu.regFile[reg] & bob16->cpu.regFile[reg1];
            break;
        case 1:
            // dest = reg & imm
            imm = word & 0xF;
            bob16->cpu.regFile[dest] = bob16->cpu.regFile[reg] & imm;
            break;
        case 2:
            // dest &= reg
            bob16->cpu.regFile[dest] &= bob16->cpu.regFile[reg];
            break;
        case 3:
            // dest &= imm
            imm = word & 0x7F;
            bob16->cpu.regFile[dest] &= imm;
            break;
    }
}

static void notInstruction(BOB16 *bob16, bobWord word) {
    if ((word >> 11 & 1) == 0) {
        size_t dest = (word >> 7) & 0x7;
        if ((word >> 10 & 1) == 0) {
            size_t src = (word >> 4) & 0x7;
            bob16->cpu.regFile[dest] = ~bob16->cpu.regFile[src];
        } else {
            bobWord imm = word & 0x7F;
            bob16->cpu.regFile[dest] = ~imm;
        }
    } else {
        size_t dest = (word >> 8) & 0x7;
        bob16->cpu.regFile[dest] = ~bob16->cpu.regFile[dest];
    }
}

static void execute(BOB16 *bob16) {
    bobWord word = bob16->ram.memory[bob16->cpu.programCounter++];
    switch (getOpcode(word)) {
        case INS_NOP:
            return;
        case INS_ADD:
            addInstruction(bob16, word);
            return;
        case INS_AND:
            andInstruction(bob16, word);
            return;
        case INS_NOT:
            notInstruction(bob16, word);
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
    execute(bob16);
}

void run(BOB16 *bob16) {
    while (true) {
        clockCycle(bob16);
    }
}

#ifdef TEST

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
    bob16->ram.memory[0] = 0x1014;
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
    bob16->cpu.regFile[1] = 0xFFFF;
    bob16->cpu.regFile[2] = 0x0F0F;
    bob16->ram.memory[0] = 0x2014;
    clockCycle(bob16);
    if (bob16->cpu.regFile[0] != 0x0F0F) {
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
    bob16->ram.memory[1] = 0x347A;
    clockCycle(bob16);
    if (bob16->cpu.regFile[0] != ~0x7A) {
        fprintf(stderr, "`reg = ~imm` does not work\n");
        fprintf(stderr, "r0 should be 0xFF85, r0 is %X\n", bob16->cpu.regFile[0]);
        return 1;
    }

    // ~=reg
    bob16->cpu.regFile[0] = 0x0F0F;
    bob16->ram.memory[2] = 0x3800; // not r0
    clockCycle(bob16);
    if (bob16->cpu.regFile[0] != ~0x0F0F) {
        fprintf(stderr, "`~=reg` does not work\n");
        fprintf(stderr, "r0 should be 0xF0F0, r0 is %X\n", bob16->cpu.regFile[0]);
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
        hadError = 1;
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
        hadError = 1;
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
        hadError = 1;
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

#endif // TEST
