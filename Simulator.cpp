#include "Simulator.h"
void Simulator :: loadprogram (std::vector<uint16_t> program ) {
    std :: vector<uint8_t> bytes;
    for (int i = 0 ; i < program.size() ; i++)
    {
        bytes.push_back(program[i] >> 8);
        bytes.push_back(program[i] & 0XFF);

    }
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
    void Simulator::runAll()
{
    while (programmed && !cpu.isHalted())
    {
        if (!runstep())
            break;
    }
}
    void Simulator :: reset()
    {
        cpu.reset();
        memory.clear();
        programmed = false;

    }
    bool Simulator :: is_programmed()
    {
        return programmed;
    }