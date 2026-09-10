#ifndef _BOB16_INSTRUCTION_H
#define _BOB16_INSTRUCTION_H

typedef enum Instruction {
    INS_NOP,
    INS_ADD,
    INS_AND,
    INS_NOT,
    INS_LD,
    INS_LDI,
    INS_LDR,
    INS_ST,
    INS_STI,
    INS_STR,
    INS_BR,
    INS_JMP,
    INS_JSR,
    INS_JSSR,
    INS_LEA,
    INS_RET,
    INS_TRAP,
} Instruction;

#endif
