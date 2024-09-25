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

uint32_t RegDealing(RegisterFile rf, int reg_write, unsigned int rs1,
    unsigned int rs2, unsigned int rd, uint32_t OutData, uint32_t choose)
{
    // 读取 rs1 和 rs2 寄存器的值
    uint32_t readData1 = read_register(&rf, rs1);
    uint32_t readData2 = read_register(&rf, rs2);

    // 如果 reg_write 为 1，写入 rd 寄存器
    if (reg_write) {
        write_register(&rf, rd, OutData);
    }

    // 返回 ALU 需要的读取数据（可以返回 rs1 或 rs2 的值，这里选择返回 rs1）
    if(choose == 1) {
        return readData1;
    }else {
        return readData1;
    }
}