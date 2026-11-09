#ifndef MEMORY_H
#define MEMORY_H
#include <cstdint>
#include <vector>
#include <string>

class Memory
{
private:
    uint8_t cells[256];
    uint8_t screenAddress = 0xFF;
    std::string screenOutput;

public:
    Memory();
    uint8_t read(uint8_t address);
    void write(uint8_t address, uint8_t value);
    void loadProgram(std::vector<uint8_t> program);
    void clear();
    void outputToScreen(uint8_t value);
    const std::string& getScreenOutput() const { return screenOutput; }
};

#endif