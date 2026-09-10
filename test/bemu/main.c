#include "../../src/bemu/computer.h"
#include "../../src/bemu/instruction.h"

#include <stdio.h>

int testAddInstruction(BOB16 *bob16) {
    return 0;
}

int main() {
    BOB16 bob16;
    initBOB16(&bob16);

    int hadError = 0;

    if (testAddInstruction(&bob16)) {
        fprintf(stderr, "Add instruction doesn't work\n");
    }

    if (!hadError) {
        printf("No errors found\n");
    }

    return hadError;
}
