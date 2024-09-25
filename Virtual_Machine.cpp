//###########################################//
//          My Virtual Machine               //
//      Haibin Lai, SUSTech HPC Lab          //
//###########################################//

#include <cstdint>
#include <cstdio>

#include "CPU.h"

#define uint32_t unsigned int

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

///////////////////// Imm Gen ////////////////
///
///@date 2024/9/26                          //
///
//////////////////////////////////////////////



uint32_t generate_immediate(unsigned int instruction) {
    uint32_t imm;

    unsigned int opcode = instruction & 0x7F;
    unsigned int funct3 = (instruction >> 12) & 0x07;
    unsigned int funct7 = (instruction >> 25) & 0x7F;

    switch (opcode) {
        case 0x13: // I-type (Immediate)
            imm = (instruction >> 20) & 0xFFF; // 12-bit immediate
        break;

        case 0x03: // I-type Load
            imm = (instruction >> 20) & 0xFFF; // 12-bit immediate
        break;

        case 0x23: // S-type Store
            imm = ((instruction >> 25) & 0x7F) << 5 | ((instruction >> 7) & 0x1F); // 12-bit immediate
        break;

        case 0x63: // B-type Branch
            imm = ((instruction >> 31) & 0x1) << 12 |
                       ((instruction >> 25) & 0x3F) << 5 |
                       ((instruction >> 8) & 0xF) << 1; // 13-bit immediate
        break;

        case 0x37: // U-type (LUI)
        case 0x17: // U-type (AUIPC)
            imm = instruction & 0xFFFFF000; // 20-bit immediate
        break;

        case 0x6F: // J-type (JAL)
            imm = ((instruction >> 31) & 0x1) << 20 |
                       ((instruction >> 21) & 0x3FF) << 1 |
                       ((instruction >> 20) & 0x1) << 11 |
                       ((instruction >> 12) & 0xFF) << 12; // 21-bit immediate
        break;

        default: // 未知指令
            imm = 0;
        printf("Unknown opcode: %02X\n", opcode);
        break;
    }

    return imm;
}


uint32_t pc = 0x00400000; /* program counter */
uint32_t CPU_status = 1;

uint32_t OutData = 0;


uint32_t Ins_MMU(uint32_t pc) {
    return pc - 0x00400000;
}


int main() {
    uint32_t instructions[40] = {
          0x00400893, // // li a7, 4
          0x0fc10517, // // la a0, prompt1
          0xffc50513,  //
          0x00000073,  // ecall
          0x00500893,  // li a7, 5
          0x00000073,  // ecall
          0x00a002b3,  // mv t0, a0
          0x00400893,  // li a7, 4
          0x0fc10517,  // la a0, prompt2
          0xfe050513,  //
          0x00000073,  // ecall
          0x00500893,  // li a7, 5
          0x00000073,  // ecall
          0x00a00333,  // mv t1, a0
          0x006283b3,  // add t2, t0, t1
          0x00400893,  // li a7, 4
          0x0fc10517,  // la a0, result
          0xfc050513,  //
          0x00000073,  // ecall
          0x00700533,  // mv a0, t2
          0x00100893,  // li a7, 1
          0x00000073,  // ecall
          0x00a00893,  // li a7, 10
          0x00000073   // ecall
    };

    uint32_t running = CPU_status;
    RegisterFile rf;
    init_registers(&rf);
    pc = 0x00400000;

    while(running) {
        uint32_t instruction_addr = Ins_MMU(pc);

        // fetch
        uint32_t instruction = instructions[instruction_addr];

        // decode
        Instruction inst = decode_instruction(instruction);

        // control
        ControlSignals control = generate_control(inst);
        uint32_t imm_gen = generate_immediate(instruction);
        uint32_t read_data[2] = RegDealing();


    }


}