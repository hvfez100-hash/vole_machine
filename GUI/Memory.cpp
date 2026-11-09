#include "Memory.h"
#include <iostream>

Memory::Memory() {
    clear();
}

uint8_t Memory::read(uint8_t address){
    return cells[address];
}
void Memory::write(uint8_t address , uint8_t value){
    cells[address]=value;
    if (address==screenAddress){
        outputToScreen (value);
    }
}
void Memory::loadProgram(std::vector<uint8_t> program){
    clear();
    size_t loadsize=program.size();
    if(loadsize>256){
        loadsize=256;
    }
    for(size_t i=0 ; i<loadsize ; i++){
        cells[i]=program[i];
    }
}
void Memory::clear(){
    for(int i=0 ; i<256 ; i++){
        cells[i]=0;
    }
}
void Memory::outputToScreen(uint8_t value){
    int number=value;
    std::cout<<number;
}
