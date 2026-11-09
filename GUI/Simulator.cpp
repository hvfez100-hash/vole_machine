#include "Simulator.h"

void Simulator::loadprogram(std::vector<uint16_t> program)
{
    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < program.size(); i++)
    {
        bytes.push_back(program[i] >> 8);
        bytes.push_back(program[i] & 0xFF);
    }
    cpu.reset();
    memory.loadProgram(bytes);
    programmed = true;
}

bool Simulator::runstep()
{
    if (!programmed)
        return false;
    if (!cpu.fetch(memory))
        return false;
    cpu.executeInstruction(memory);
    if (cpu.hasError())
        return false;
    return true;
}

bool Simulator::runAll(int maxSteps)
{
    int steps = 0;
    while (programmed && !cpu.isHalted())
    {
        if (steps++ >= maxSteps)
            return false;
        if (!runstep())
            break;
    }
    return true;
}

void Simulator::reset()
{
    cpu.reset();
    memory.clear();
    programmed = false;
}

bool Simulator::is_programmed()
{
    return programmed;
}

uint8_t Simulator::readMemory(uint8_t address)
{
    return memory.read(address);
}

uint8_t Simulator::getRegister(uint8_t index)
{
    return cpu.getRegister(index).getValue();
}