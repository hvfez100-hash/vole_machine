#ifndef SIMULATOR_H
#define SIMULATOR_H
#include "CPU.h"
#include "program.h"
#include "Memory.h"
#include <vector>
#include <cstdint>
#include <string>
#include <cstddef>

class Simulator {
private:
    CPU cpu;
    Memory memory;
    bool programmed = false;

public:
    void loadprogram(std::vector<uint16_t> program);
    bool runstep();
    bool runAll(int maxSteps = 100000);
    void reset();
    bool is_programmed();
    uint8_t readMemory(uint8_t address);
    uint8_t getRegister(uint8_t index);

    bool isHalted()          { return cpu.isHalted(); }
    bool hasError()          { return cpu.hasError(); }
    std::string getError()   { return cpu.getErrorMessage(); }
    uint8_t getPC()          { return cpu.getPC(); }
    uint16_t getIR()         { return cpu.getIR(); }
    std::string getScreen()  { return memory.getScreenOutput(); }
};

#endif