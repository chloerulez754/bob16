#ifndef _BOB16_COMPUTER_H
#define _BOB16_COMPUTER_H

#include <stdint.h>
#include <stdlib.h>

#define REG_COUNT 8
#define RAM_MAX 0x10000

typedef int16_t bobWord;

typedef struct CPU {
    bobWord regFile[REG_COUNT];
    size_t programCounter;
} CPU;

typedef struct RAM {
    bobWord memory[RAM_MAX];
} RAM;

typedef struct BOB16 {
    CPU cpu;
    RAM ram;
} BOB16;

void initBOB16(BOB16 *bob16);

void run(BOB16 *bob16);

#endif
