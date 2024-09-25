// #include <cstdio>
// #include <process.h>
// #include <wchar.h>

// #include "Virtual_Machine.h"
//
// #define register uint16_t
// #define imm uint16_t
//
// /**
//  * Register
//  */
// enum {
//     R_R0 = 0,
//     R_R1,
//     R_R2,
//     R_R3,
//     R_R4,
//     R_R5,
//     R_R6,
//     R_R7,
//     R_PC, /* program counter */
//     R_COND,/* program condition */
//     R_COUNT
// };
//
// /**
//  * OPcode
//  */
// enum {
//     OP_BR = 0, /* branch */
//     OP_ADD,    /* add  */
//     OP_LD,     /* load */
//     OP_ST,     /* store */
//     OP_JSR,    /* jump register */
//     OP_AND,    /* bitwise and */
//     OP_LDR,    /* load register */
//     OP_STR,    /* store register */
//     OP_RTI,    /* unused */
//     OP_NOT,    /* bitwise not */
//     OP_LDI,    /* load indirect */
//     OP_STI,    /* store indirect */
//     OP_JMP,    /* jump */
//     OP_RES,    /* reserved (unused) */
//     OP_LEA,    /* load effective address */
//     OP_TRAP    /* execute trap */
// };
//
// /**
//  * Condition
//  */
// enum {
//     FL_POS = 1 << 0, /* P */
//     FL_ZRO = 1 << 1, /* Z */
//     FL_NEG = 1 << 2, /* N */
// };
//
// /**
//  * Trap
//  */
// enum Trap
// {
//     TRAP_GETC = 0x20,  /* get character from keyboard */
//     TRAP_OUT = 0x21,   /* output a character */
//     TRAP_PUTS = 0x22,  /* output a word string */
//     TRAP_IN = 0x23,    /* input a string */
//     TRAP_PUTSP = 0x24, /* output a byte string */
//     TRAP_HALT = 0x25   /* halt the program */
// };
//
// /**
//  * Memory
//  */
// enum Memory
// {
//     MR_KBSR = 0xFE00,  /* keyboard status */
//     MR_KBDR = 0xFE02,  /* keyboard data */
//     MR_DSR = 0xFE04,   /* display status */
//     MV_DDR = 0xFE06,   /* display data */
//     MCR = 0xFFFE       /* machine control */
// };
//
// register Reg[R_COUNT];
//
// register sign_extend(register x, int const bit_count) {
//     // if the biggest bit in x is 1
//     if(x >> (bit_count - 1) & 1) {
//         // set all after biggest bit in x to 1
//         x |= (0xFFFF << bit_count);
//     }
//     return x;
// }
//
// void update_flags(register const r) {
//     if (Reg[r] == 0) {
//         Reg[R_COND] = FL_ZRO;
//     }
//     else if (reg[r] >> 15) { /* a 1 in the left-most bit indicates negative */
//         Reg[R_COND] = FL_NEG;
//     } else {
//         Reg[R_COND] = FL_POS;
//     }
// }
//
// void ADD() {
//     // register r0 = (instr >> 9) & 0x7; /* destination register */
//     // register r1 = (instr >> 6) & 0x7; /* first operand */
//     // imm imm_flag = (instr >> 5) & 0x1; /* immediate mode */
//     //
//     // if(imm_flag) {
//     //     imm imm5 = sign_extend(instr & 0x1F, 5);
//     }
// }
//
// int main(int argc, char **argv)
// {
//     vm_ctx *curr_vm;
//     if (argc == 2)
//     {
//         curr_vm = init_vm(argv[1]);
//     }
//     else
//     {
//         fprintf(stderr, "Usage: %s [program]\n", argv[0]);
//         exit(-1);
//     }
//
//     int running = 1;
//     while (running)
//     {
//         running = execute_inst(curr_vm);
//     }
//     destory_vm(curr_vm);
//     return 0;
// }
//
