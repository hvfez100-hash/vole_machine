#include "Instruction.h"

Instruction::Instruction(uint16_t rawValue){
this->rawValue = rawValue;
opcode=(rawValue>>12)& 0x0F;
operand1=(rawValue>>8)& 0x0F;
operand2=(rawValue>>4)& 0x0F;
operand3=(rawValue>>0)& 0x0F;
}

uint8_t Instruction::getOpcode(){
return opcode;
}

uint8_t Instruction::getOperand1(){
return operand1;
}

uint8_t Instruction::getOperand2(){
return operand2;
}

uint8_t Instruction::getOperand3(){
return operand3;
}
