//
// Created by pc on 9/26/2024.
//

#ifndef DECODE_H
#define DECODE_H

#define uint32_t unsigned int


#include "decode.cpp"
#include "immediate.cpp"
#include "control.cpp"
#include "register.cpp"

//////////////////////////// MACRO //////////////

/**
 * Register
 */
enum {
    X0 = 0,
    RA, /* return addr */
    SP, /* stack pointer */
    GP, /* global pointer */
    TP, /* thread pointer */
    t0, /* tmp */
    t1, /* tmp */
    t2, /* tmp */
    fp, /* frame pointer / save reg */
    s1, /* save reg */
    a0, /* func argument */
    a1, /* func argument */
    a2, /* func argument */
    a3, /* func argument */
    a4, /* func argument */
    a5, /* func argument */
    a6, /* func argument */
    a7, /* func argument */
    s2, /* save reg */
    s3, /* save reg */
    s4, /* save reg */
    s5, /* save reg */
    s6, /* save reg */
    s7, /* save reg */
    s8, /* save reg */
    s9, /* save reg */
    s10, /* save reg */
    s11, /* save reg */
    t3, /* tmp */
    t4, /* tmp */
    t5, /* tmp */
    t6, /* tmp */


};

/**
 * OPcode
 */
enum {
    OP_BR = 0, /* branch */
    OP_ADD,    /* add  */
    OP_LD,     /* load */
    OP_ST,     /* store */
    OP_JSR,    /* jump register */
    OP_AND,    /* bitwise and */
    OP_LDR,    /* load register */
    OP_STR,    /* store register */
    OP_RTI,    /* unused */
    OP_NOT,    /* bitwise not */
    OP_LDI,    /* load indirect */
    OP_STI,    /* store indirect */
    OP_JMP,    /* jump */
    OP_RES,    /* reserved (unused) */
    OP_LEA,    /* load effective address */
    OP_TRAP    /* execute trap */
};

/**
 * Condition
 */
enum {
    FL_POS = 1 << 0, /* P */
    FL_ZRO = 1 << 1, /* Z */
    FL_NEG = 1 << 2, /* N */
};




#endif //DECODE_H
