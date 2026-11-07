#ifndef SIMULATOR_H
#define SIMULATOR_H
#include "CPU.h"
#include "program.h"
#include "Memory.h"
#include<vector>
#include<cstdint>
#include<string>
#include <cstddef>
class Simulator {
    private:
    CPU cpu;
    Memory memory;
    bool programmed = false;
    public :
    void loadprogram (std::vector<uint16_t> program );
    bool runstep();
    void runAll();
    void reset();
    bool is_programmed();
    uint8_t readMemory( uint8_t address );
    uint8_t getRegister( uint8_t index );
};
#endif
