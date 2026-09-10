# bob16

The bob16 is a computer that was made by [misterbob](https://www.youtube.com/@misterbob360), and the original bob16 emulator is at [github.com](https://github.com/somerandomviolinkid/bob16)

This emulator should do the same things as the original bob16 emulator

There isn't much documentation on how the binary of the instructions are stored, so until I read through the source then this is a best guess

Known differences: PUTS (TRAP 2) does not add an extra newline, unlike the original bob16 emulator

## Instructions

dest and src are both registers.
pc points to the current instruction being executed.

NOP -> Does nothing

0b0000_0000_0000_0000

### Arithmetic instructions

ADD -> dest = src0 + src1
src1 can be an imm
imm is a 4-bit signed number (min: -8, max: 7)
e.g. `add r0 r1 r2` OR `add r0 r1 5`

0b0001_0mdd_dzzz_oooi
m: mode 0=reg 1=imm;
d: dest reg;
z: src0 reg;
o: src1 reg/imm;
i: 0/imm;


ADD -> dest += src
src1 can be an imm
imm is a 7-bit signed number (min: -64, max: 63)
e.g. `add r0 r1` OR `add r0 5`

0b0001_1mdd_dsss_iiii
m: mode 0=reg 1=imm;
d: dest;
s: src/imm;
i: 0/imm;


AND -> dest = src0 & src1
src1 can be an imm
imm is a 4-bit signed number (min: -8, max: 7)
e.g. `and r0 r1 r2` OR `and r0 r1 5`

0b0010_0mdd_dzzz_oooi
m: mode 0=reg 1=imm;
d: dest;
z: src0;
o: src1/imm;
i: 0/imm;


AND -> dest &= src
src can be an imm
imm is a 7-bit signed number (min: -64, max: 63)
e.g. `and r0 r1` OR `and r0 5`

0b0010_1mdd_dsss_iiii
m: mode 0=reg 1=imm;
d: dest;
s: src/imm;
i: 0/imm;


NOT -> dest = ~src
src can be an imm
imm is a 7-bit signed number (min: -64, max: 63)
e.g. `not r0 r1` OR `not r0 5`

0b0011_0mdd_dsss_iiii
m: mode 0=reg 1=imm;
d: dest;
s: src/imm;
i: 0/imm;


NOT -> ~dest
dest can NOT be an immediate value
e.g. `not r0`

0b0011_1ddd_0000_0000
d: dest;



### Memory instructions

LD -> dest = ram[pc + imm]
imm is a 9-bit signed number (min: -256, max: 255)
e.g. `ld r0 5`

0b0100_dddi_iiii_iiii
d: dest;
i: imm;


LDI -> dest = ram[ram[pc + imm]]
imm is a 9-bit signed number (min: -256, max: 255)
e.g. `ldi r0 5`

0b0101_dddi_iiii_iiii
d: dest;
i: imm;


LDR -> dest = ram[src + imm]
imm is a 6-bit signed number (min: -32, max: 31)
e.g. `ldr r0 r1 5`

0b0110_ddds_ssii_iiii
d: dest;
s: src;
i: imm;


ST -> ram[pc + imm] = src
imm is a 9-bit signed number (min: -256, max: 255)
e.g. `st r0 5`

0b0111_sssi_iiii_iiii
s: src;
i: imm;


STI -> ram[ram[pc + imm]] = src
imm is a 9-bit signed number (min: -256, max: 255)
e.g. `sti r0 5`

0b1000_sssi_iiii_iiii
s: src;
i: imm;


STR -> ram[dest + imm] = src
imm is a 6-bit signed number (min: -32, max: 31)
e.g. `str r0 r1 5`

0b1001_sssd_ddii_iiii
s: src;
d: dest;
i: imm;



### Control flow

BR -> pc = pc + imm IF cpu_flags = src_flags
imm is a 9-bit signed number (min: -256, max: 255)
src_flags are any combination of n (negative), z (zero), or p (positive)
e.g. `br np 5`

0b1010_nzpi_iiii_iiii
n: negative (1 to jump);
z: zero (1 to jump);
p: positive (1 to jump);
i: imm;


JMP -> pc = src
e.g. `jmp r0`

0b1011_sss0_0000_0000
s: src;


JSR -> r7 = pc; pc += src
src can be an imm
imm is a 11-bit number (min: -1024, max: 1023)
e.g. `jsr r0` OR `jsr 5`

0b1100_msss_iiii_iiii
m: mode 0=reg 1=imm;
s: src/imm;
i: 0/imm;


LEA -> src = pc
e.g. `lea r0`

0b1101_sss0_0000_0000
s: src;


RET -> pc = r7
e.g. `ret`

0b1110_0000_0000_0000



### High level functions

TRAP -> TRAP_VEC[imm]()
imm is a 12-bit unsigned number (min: 0, max: 4096)
e.g. `trap 0`

0b1111_iiii_iiii_iiii
i: imm



## Trap functions

TRAP 0 -> HALT
Halts the program execution (must use at end of program)

TRAP 1 -> PUTC
Puts a single ASCII character from r0 on the screen

TRAP 2 -> PUTS
Puts a null terminated string on the screen
The string starts at r0
NOTE: the string doesn't have a newline appended, so the programmer will have to add that yourself

TRAP 3 -> GETS
Gets a null terminated string from the user
The length of the string needed is stored in r1, the buffer that the string will be stored in starts at r0
NOTE: the inputted string has an extra newline at the end

## License

This repository is licensed under the MIT License

The [original bob16 repository](https://github.com/somerandomviolinkid/bob16) is not licensed under any license
