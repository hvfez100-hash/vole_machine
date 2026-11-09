#ifndef VOLE_MACHINE_CPU_H
#define VOLE_MACHINE_CPU_H

#include <cstdint>
#include <string>
#include "Register.h"
#include "ALU.h"
#include "Memory.h"
#include "Instruction.h"

class CPU {
private:
    Register registers[16];
    std::uint8_t pc = 0;
    std::uint16_t ir = 0;
    bool halted= false;
    bool executionError = false;
    std::string errorMessage;
    ALU alu;
public:
    bool fetch(Memory& memory);
    Instruction decode(std::uint16_t rawValue);
    void executeInstruction(Memory& memory);
    void reset();
    bool isHalted() const;
    bool hasError() const { return executionError; }
    std::string getErrorMessage() const { return errorMessage; }
    std::uint8_t getPC() const;
    std::uint16_t getIR() const;
    Register& getRegister(std::uint8_t index);
};

#endif