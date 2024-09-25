//###########################################//
//          My Virtual Machine               //
//      Haibin Lai, SUSTech HPC Lab          //
//###########################################//

#include <cstdint>
#include <cstdio>

#include "CPU.h"



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
        uint32_t read_data1 = RegDealing(
            rf, control.reg_write, inst.rs1, inst.rs2,
            inst.rd, OutData, 1);
        uint32_t read_data2 = RegDealing(
    rf, control.reg_write, inst.rs1, inst.rs2,
    inst.rd, OutData, 2);


    }


}