//###########################################//
//          My Virtual Machine               //
//      Haibin Lai, SUSTech HPC Lab          //
//###########################################//

//
// Created by pc on 9/26/2024.
//

// #include "CPU.h"

//////////////////////////// MACRO //////////////

#include <iostream>
#include <ostream>
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

////////////// decode ////////////////

typedef struct {
    unsigned int opcode;
    unsigned int rd;
    unsigned int funct3;
    unsigned int rs1;
    unsigned int rs2;
    unsigned int funct7;
    unsigned int imm;
} Instruction;

Instruction decode_instruction(unsigned int instruction) {
    Instruction inst;

    // 提取字段
    inst.opcode = instruction & 0x7F;                // 7 位 opcode
    inst.rd = (instruction >> 7) & 0x1F;              // 5 位 rd
    inst.funct3 = (instruction >> 12) & 0x07;         // 3 位 funct3
    inst.rs1 = (instruction >> 15) & 0x1F;             // 5 位 rs1
    inst.rs2 = (instruction >> 20) & 0x1F;             // 5 位 rs2
    inst.funct7 = (instruction >> 25) & 0x7F;          // 7 位 funct7

    // 处理立即数（这里以 I-type 为例）
    if (inst.opcode == 0x03 || inst.opcode == 0x13) { // I-type
        inst.imm = (instruction >> 20) & 0xFFF;         // 12 位立即数
    } else if (inst.opcode == 0x23) { // S-type
        inst.imm = ((instruction >> 25) & 0x7F) << 5 | ((instruction >> 7) & 0x1F); // 12 位立即数
    } else {
        inst.imm = 0; // 其他类型未处理
    }

    return inst;
}
//
// Created by pc on 9/26/2024.
//


// #include "CPU.h"
///////////////////// Imm Gen ////////////////
///
///@date 2024/9/26                          //
///
//////////////////////////////////////////////

// #define uint32_t unsigned int
#include <cstdint>
#include <cstdio>

uint32_t generate_immediate(unsigned int instruction) {
    uint32_t imm;

    unsigned int opcode = instruction & 0x7F;
    // unsigned int funct3 = (instruction >> 12) & 0x07;
    // unsigned int funct7 = (instruction >> 25) & 0x7F;

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
        printf("Unknown opcode: %02X from imm unit\n", opcode);
        break;
    }

    return imm;
}
#include "CPU.h"

//////////////// Control ////////////////////

typedef struct {
    int reg_write;     // 控制信号：寄存器写入使能
    int alu_src;       // 控制信号：ALU 源选择
    int mem_read;      // 控制信号：内存读取
    int mem_write;     // 控制信号：内存写入
    int branch;        // 控制信号：分支使能
    int alu_op;        // ALU 操作选择
    int mem_to_reg;
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
            printf("Unknown opcode: %02X from control unit\n", inst.opcode);
        break;
    }

    return control;
}

/////////////////////////////// ALU control //////////////////
enum ALUControlSignal {
    CTRL_ADD,
    CTRL_SUB,
    CTRL_AND,
    CTRL_OR,
    CTRL_XOR,
    CTRL_SLT,
    CTRL_SLTU,
    CTRL_NOP,
};

enum ALUOp {
    ALU_ADD,
    ALU_SUB,
    ALU_AND,
    ALU_OR,
    ALU_XOR,
    ALU_SLT,
    ALU_SLTU,
    ALU_NOP, // No operation
};

// 从指令和 ALU 操作信号生成 ALU 控制信号
ALUControlSignal generateControlSignal(uint32_t instruction, ALUOp aluOp) {
    // 假设 instruction 的结构如下：
    // - opcode: instruction[6:0]
    // - funct3: instruction[14:12]
    // - funct7: instruction[31:25]

    uint32_t opcode = instruction & 0x7F;
    uint32_t funct3 = (instruction >> 12) & 0x07;
    uint32_t funct7 = (instruction >> 25) & 0x7F;

    switch (aluOp) {
        case ALU_ADD:
            return (funct7 == 0 && funct3 == 0) ? CTRL_ADD : CTRL_NOP; // ADD
        case ALU_SUB:
            return (funct7 == 0x20 && funct3 == 0) ? CTRL_SUB : CTRL_NOP; // SUB
        case ALU_AND:
            return (funct7 == 0 && funct3 == 7) ? CTRL_AND : CTRL_NOP; // AND
        case ALU_OR:
            return (funct7 == 0 && funct3 == 6) ? CTRL_OR : CTRL_NOP; // OR
        case ALU_XOR:
            return (funct7 == 0 && funct3 == 4) ? CTRL_XOR : CTRL_NOP; // XOR
        case ALU_SLT:
            return (funct7 == 0 && funct3 == 2) ? CTRL_SLT : CTRL_NOP; // SLT
        case ALU_SLTU:
            return (funct7 == 0 && funct3 == 3) ? CTRL_SLTU : CTRL_NOP; // SLTU
        default:
            return CTRL_NOP; // 默认无操作
    }
}


// 从指令生成 ALU 操作信号
ALUOp generateALUOp(uint32_t instruction) {
    // 假设 instruction 的结构如下：
    // - opcode: instruction[6:0]
    // - funct3: instruction[14:12]
    // - funct7: instruction[31:25]

    uint32_t opcode = instruction & 0x7F;
    uint32_t funct3 = (instruction >> 12) & 0x07;
    uint32_t funct7 = (instruction >> 25) & 0x7F;

    switch (opcode) {
        case 0x33: // R-type 指令（例如：ADD, SUB, AND, OR, XOR, SLT, SLTU）
            if (funct3 == 0) {
                return (funct7 == 0) ? ALU_ADD : ALU_SUB; // 0: ADD, 0x20: SUB
            } else if (funct3 == 7) {
                return ALU_AND;
            } else if (funct3 == 6) {
                return ALU_OR;
            } else if (funct3 == 4) {
                return ALU_XOR;
            } else if (funct3 == 2) {
                return ALU_SLT;
            } else if (funct3 == 3) {
                return ALU_SLTU;
            }
        break;

        // 可以添加其他类型的指令处理，例如 I-type 和 S-type

        default:
            return ALU_NOP; // 默认无操作
    }

    return ALU_NOP; // 默认无操作
}

//
// Created by pc on 9/26/2024.
//


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
inline void write_register(RegisterFile *rf, unsigned int rd, unsigned int data) {
    if (rd != 0) { // 寄存器 0 不可写
        rf->registers[rd] = data;
    }
}

// 读寄存器
inline unsigned int read_register(RegisterFile *rf, unsigned int rs) {
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
        return readData2;
    }
}

////////////////////// ALU ///////////////
///

// 定义操作码
enum Operation {
    ADD,    // 加法
    SUB,    // 减法
    AND,    // 按位与
    OR,     // 按位或
    XOR,    // 按位异或
    SLT,    // 小于
    SLTU,   // 无符号小于
     // 其他操作可以根据需要添加
 };

// 执行 ALU 操作
uint32_t ALU(uint32_t operand1, uint32_t operand2, ALUOp op) {
    switch (op) {
        case ALU_ADD:
            return operand1 + operand2;
        case ALU_SUB:
            return operand1 - operand2;
        case ALU_AND:
            return operand1 & operand2;
        case ALU_OR:
            return operand1 | operand2;
        case ALU_XOR:
            return operand1 ^ operand2;
        case ALU_SLT:
            return (operand1 < operand2) ? 1 : 0; // 小于返回 1
        case ALU_SLTU:
            return (static_cast<uint32_t>(operand1) < static_cast<uint32_t>(operand2)) ? 1 : 0; // 无符号小于
        default:
            std::cerr << "Unknown operation!" << std::endl;
        return 0;
    }
}

uint32_t ALU_MUX(uint32_t operand2, uint32_t immgen, uint32_t alu_src) {
    if(alu_src) {
        return immgen;
    }else {
        return operand2;
    }
}

uint32_t PC_Control(uint32_t immgen, int branch , uint32_t iszero) {

    if(iszero && branch) {
        return immgen;
    }else {
        return 1;
    }
}

uint32_t Alu_Zero(uint32_t result) {
    if(result == 0) {
        return 1;
    }else {
        return 0;
    }
}

#define MEMORY_SIZE 100
uint32_t MEMORY[MEMORY_SIZE];

/////////////////////// Mem Write //////////
uint32_t Memory(uint32_t alu_result, int MemWrite, int MemRead, uint32_t write_data) {
    if(MemWrite) {
        MEMORY[alu_result] = write_data;
        return 0;
    }

    if(MemRead) {
        return MEMORY[alu_result];
    }
    return 0;
}

void MemoryInit() {
    for(int i =0; i< MEMORY_SIZE; i++) {
        MEMORY[i] = 0;
    }
}

uint32_t MemMUX(int mem_to_reg,uint32_t read_mem_data, uint32_t result) {
    if(mem_to_reg) {
        return read_mem_data;
    }else {
        return  result;
    }
}

////////////////////////


uint32_t pc = 0x00400000; /* program counter */
uint32_t CPU_status = 1;

uint32_t OutData = 0;


uint32_t Ins_MMU(uint32_t pc) {
    return pc - 0x00400000;
}


int main() {
    uint32_t program_length = 18;
    uint32_t instructions[program_length] = {
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

    };

    uint32_t running = CPU_status;
    RegisterFile rf;
    init_registers(&rf);
    MemoryInit();
    pc = 0x00400000;
    uint32_t instruction_addr = Ins_MMU(pc);

    while(instruction_addr < program_length) {
        // MMU -> pc
        instruction_addr = Ins_MMU(pc);
        // fetch
        uint32_t instruction = instructions[instruction_addr];

        // decode
        Instruction inst = decode_instruction(instruction);
        // control
        ControlSignals control = generate_control(inst);

        // imm_gen
        uint32_t imm_gen = generate_immediate(instruction);

        // read
        uint32_t read_data1 = RegDealing(
            rf, control.reg_write, inst.rs1, inst.rs2,inst.rd, OutData, 1);
        uint32_t read_data2 = RegDealing(
            rf, control.reg_write, inst.rs1, inst.rs2,inst.rd, OutData, 2);

        // ALU op
        ALUOp alu_op_code = generateALUOp(instruction);
        // ALU Signal
        ALUControlSignal alu_control_signal =  generateControlSignal(instruction,alu_op_code);
        // MUX
        uint32_t alu_op2 = ALU_MUX(read_data2, imm_gen, control.alu_src);

        // ALU
        uint32_t alu_result = ALU(read_data1, alu_op2, alu_op_code);
        uint32_t is_zero = Alu_Zero(alu_result);

        // PC control
        pc = pc + PC_Control(imm_gen, control.branch, is_zero);
        // read Data
        uint32_t read_mem_data = Memory(alu_result, control.mem_write, control.mem_read, read_data2);
        OutData = MemMUX(control.mem_to_reg, read_mem_data, alu_result);
    }


}