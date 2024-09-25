#include <stdio.h>

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

void print_instruction(Instruction inst) {
    printf("opcode: %02X\n", inst.opcode);
    printf("rd: %02X\n", inst.rd);
    printf("funct3: %02X\n", inst.funct3);
    printf("rs1: %02X\n", inst.rs1);
    printf("rs2: %02X\n", inst.rs2);
    printf("funct7: %02X\n", inst.funct7);
    printf("imm: %03X\n", inst.imm);
}

// int main() {
//     unsigned int instruction = 0x00C585B3; // 示例指令：add t0, t1, t2
//     Instruction decoded = decode_instruction(instruction);
//
//     print_instruction(decoded);
//
//     return 0;
// }
