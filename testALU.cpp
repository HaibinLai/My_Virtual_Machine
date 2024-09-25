#include <cstdint>
#include <iostream>

class ALU {
public:
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
    uint32_t execute(uint32_t operand1, uint32_t operand2, Operation op) {
        switch (op) {
            case ADD:
                return operand1 + operand2;
            case SUB:
                return operand1 - operand2;
            case AND:
                return operand1 & operand2;
            case OR:
                return operand1 | operand2;
            case XOR:
                return operand1 ^ operand2;
            case SLT:
                return (operand1 < operand2) ? 1 : 0; // 小于返回 1
            case SLTU:
                return (static_cast<uint32_t>(operand1) < static_cast<uint32_t>(operand2)) ? 1 : 0; // 无符号小于
            default:
                std::cerr << "Unknown operation!" << std::endl;
            return 0;
        }
    }
};

// int main() {
//     ALU alu;
//
//     // 测试 ALU 操作
//     uint32_t a = 10;
//     uint32_t b = 20;
//
//     uint32_t result_add = alu.execute(a, b, ALU::ADD);
//     uint32_t result_sub = alu.execute(a, b, ALU::SUB);
//     uint32_t result_and = alu.execute(a, b, ALU::AND);
//     uint32_t result_or = alu.execute(a, b, ALU::OR);
//     uint32_t result_xor = alu.execute(a, b, ALU::XOR);
//     uint32_t result_slt = alu.execute(a, b, ALU::SLT);
//     uint32_t result_sltu = alu.execute(a, b, ALU::SLTU);
//
//     std::cout << "ADD: " << result_add << std::endl;
//     std::cout << "SUB: " << result_sub << std::endl;
//     std::cout << "AND: " << result_and << std::endl;
//     std::cout << "OR: " << result_or << std::endl;
//     std::cout << "XOR: " << result_xor << std::endl;
//     std::cout << "SLT: " << result_slt << std::endl;
//     std::cout << "SLTU: " << result_sltu << std::endl;
//
//     return 0;
// }
