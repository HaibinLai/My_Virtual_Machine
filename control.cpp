//
// Created by pc on 9/26/2024.
//

#include "CPU.h"
//////////////// Control ////////////////////

typedef struct {
    int reg_write;     // 控制信号：寄存器写入使能
    int alu_src;       // 控制信号：ALU 源选择
    int mem_read;      // 控制信号：内存读取
    int mem_write;     // 控制信号：内存写入
    int branch;        // 控制信号：分支使能
    int alu_op;        // ALU 操作选择
} ControlSignals;

ControlSignals generate_control(Instruction const inst) {
    ControlSignals control;

    // 初始化控制信号
    control.reg_write = 0;
    control.alu_src = 0;
    control.mem_read = 0;
    control.mem_write = 0;
    control.branch = 0;
    control.alu_op = 0;

    // 根据 opcode生成控制信号
    switch (inst.opcode) {
        case 0x33: // R-type 指令
            control.reg_write = 1;  // 使能寄存器写入
        control.alu_op = 2;     // 设置为 R-type 的 ALU 操作
        break;

        case 0x03: // I-type Load 指令
            control.reg_write = 1;  // 使能寄存器写入
        control.mem_read = 1;   // 启用内存读取
        control.alu_src = 1;    // 选择立即数作为 ALU 源
        control.alu_op = 0;     // 设置为加载的 ALU 操作
        break;

        case 0x23: // S-type Store 指令
            control.mem_write = 1;   // 启用内存写入
        control.alu_src = 1;      // 选择立即数作为 ALU 源
        control.alu_op = 0;       // 设置为存储的 ALU 操作
        break;

        case 0x63: // B-type Branch 指令
            control.branch = 1;       // 启用分支操作
        control.alu_op = 1;       // 设置为分支的 ALU 操作
        break;

        default: // 未知指令
            printf("Unknown opcode: %02X\n", inst.opcode);
        break;
    }

    return control;
}
