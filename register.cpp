//
// Created by pc on 9/26/2024.
//
#include <cstdio>

#include "CPU.h"

//////////// Register //////////////////
#define NUM_REGISTERS 32

typedef struct {
    unsigned int registers[NUM_REGISTERS]; // 32 个寄存器
} RegisterFile;

// 初始化寄存器
void init_registers(RegisterFile *rf) {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        rf->registers[i] = 0; // 将所有寄存器初始化为 0
    }
}

// 写寄存器
void write_register(RegisterFile *rf, unsigned int rd, unsigned int data) {
    if (rd != 0) { // 寄存器 0 不可写
        rf->registers[rd] = data;
    }
}

// 读寄存器
unsigned int read_register(RegisterFile *rf, unsigned int rs) {
    return rf->registers[rs];
}

void print_registers(RegisterFile *rf) {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        printf("Register %02d: %08X\n", i, rf->registers[i]);
    }
}