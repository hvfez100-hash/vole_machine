#include "CPU.h"
bool CPU::fetch(Memory& memory){
    if (halted || executionError){
        return false;
    }
    if (pc == 255){
        executionError = true;
        errorMessage = "Program counter ran past the end of memory";
        return false;
    }
    uint8_t firstByte = memory.read(pc);
    uint8_t secondByte = memory.read(pc + 1);

    ir = (firstByte << 8) | secondByte;
    if (pc != 254){
        pc = pc + 2;
    } else{
        pc = pc + 1;
    }
    return true;
}

Instruction CPU::decode(std::uint16_t rawValue){
    return Instruction(rawValue);
}

void CPU::executeInstruction(Memory& memory){
    if (halted || executionError){
        return;
    }
    Instruction inst = decode(ir);
    uint8_t result;
    switch (inst.getOpcode()){
    case 1: {
        uint8_t address = (inst.getOperand2() << 4) | inst.getOperand3();
        uint8_t data = memory.read(address);
        registers[inst.getOperand1()].setValue(data);
        break;}
    case 2: {
        uint8_t data = (inst.getOperand2() << 4) | inst.getOperand3();
        registers[inst.getOperand1()].setValue(data);
        break;}
    case 3: {
        uint8_t address = (inst.getOperand2() << 4) | inst.getOperand3();
        uint8_t data = registers[inst.getOperand1()].getValue();
        memory.write(address, data);
        break;}
    case 4: {
        registers[inst.getOperand3()].setValue(registers[inst.getOperand2()].getValue());
        break;}
    case 5: {
        result = alu.addIntegers(registers[inst.getOperand2()].getValue(), registers[inst.getOperand3()].getValue());
        registers[inst.getOperand1()].setValue(result);
        break;}
    case 6: {
        result = alu.addFloatingPoint(registers[inst.getOperand2()].getValue(), registers[inst.getOperand3()].getValue());
        registers[inst.getOperand1()].setValue(result);
        break;}
    case 7: {
        result = alu.bitWiseOr(registers[inst.getOperand2()].getValue(), registers[inst.getOperand3()].getValue());
        registers[inst.getOperand1()].setValue(result);
        break;}
    case 8: {
        result = alu.bitWiseAnd(registers[inst.getOperand2()].getValue(), registers[inst.getOperand3()].getValue());
        registers[inst.getOperand1()].setValue(result);
        break;}
    case 9: {
        result = alu.bitWiseXor(registers[inst.getOperand2()].getValue(), registers[inst.getOperand3()].getValue());
        registers[inst.getOperand1()].setValue(result);
        break;}
    case 0xA: {
        result = alu.rotateRight(registers[inst.getOperand1()].getValue(), inst.getOperand3());
        registers[inst.getOperand1()].setValue(result);
        break;}
    case 0xB: {
        if (registers[inst.getOperand1()].getValue() == registers[0].getValue()){
            pc = (inst.getOperand2() << 4) | inst.getOperand3();
        }
        break; }
    case 0xC: {
        halted = true;
        break;}
    default: {
        executionError = true;
        errorMessage = "Invalid opcode";}
    }
}

void CPU::reset(){
    for (int i = 0; i < 16; i++) {
        registers[i].reset();
    }
    pc = 0;
    ir = 0;
    halted = false;
    executionError = false;
    errorMessage = "";
}

bool CPU::isHalted() const{
    return halted;
}

std::uint8_t CPU::getPC() const{
    return pc;
}

std::uint16_t CPU::getIR() const{
    return ir;
}

Register& CPU::getRegister(std::uint8_t index){
    return registers[index];
}