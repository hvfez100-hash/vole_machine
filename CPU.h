#ifndef VOLE_MACHINE_CPU_H
#define VOLE_MACHINE_CPU_H

#include <stdint.h>
#include <string>
#include "Register.h"
#include "ALU.h"
#include "Memory.h"
#include "Instruction.h"

class CPU {
private:
    Register registers[16];
    std::uint8_t pc;
    std::uint16_t ir;
    bool halted;
    bool executionError;
    std::string errorMessage;
    ALU alu;
public:
    bool fetch(Memory& memory);
    Instruction decode(std::uint16_t rawValue);
    void executeInstruction(Memory& memory);
    void reset();
    bool isHalted();
    bool hasError() const { return executionError; }
    std::uint8_t getPC();
    std::uint16_t getIR();
    Register& getRegister(std::uint8_t index);
};

#endif
