#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <cstdint>

class Instruction
{
private:
    uint16_t rawValue;
    uint8_t opcode;
    uint8_t operand1;
    uint8_t operand2;
    uint8_t operand3;

public:

    Instruction(uint16_t rawValue);
    uint8_t getOpcode();
    uint8_t getOperand1();
    uint8_t getOperand2();
    uint8_t getOperand3();

};

#endif