#include <stdio.h>
#include "computer.h"

int main(int argc, const char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Wrong amount of arguments: expected 2 arguments\n");
        return 1;
    }

    FILE *inFile = fopen(argv[1], "rb");
    BOB16 bob16;
    initBOB16(&bob16);
    size_t bytesRead = fread(bob16.ram.memory, sizeof(bobWord), RAM_MAX, inFile);

    if (!bytesRead) {
        fprintf(stderr, "Failed to read file\n");
    }

    run(&bob16);

    printf("RUN SUCCESS\n");
}
