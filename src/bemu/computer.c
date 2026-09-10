#include "computer.h"

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
