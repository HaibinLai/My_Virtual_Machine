//
// Created by pc on 9/26/2024.
//


#include "CPU.h"
///////////////////// Imm Gen ////////////////
///
///@date 2024/9/26                          //
///
//////////////////////////////////////////////



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
        printf("Unknown opcode: %02X\n", opcode);
        break;
    }

    return imm;
}