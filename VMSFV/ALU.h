#ifndef VOLE_MACHINE_ALU_H
#define VOLE_MACHINE_ALU_H
#include<stdint.h>

class ALU {
    public:
    uint8_t addIntegers(uint8_t a, uint8_t b) const;
    uint8_t addFloatingPoint(uint8_t a, uint8_t b) const;
    uint8_t bitWiseAnd(uint8_t a, uint8_t b) const;
    uint8_t bitWiseOr(uint8_t a, uint8_t b) const;
    uint8_t bitWiseXor(uint8_t a, uint8_t b) const;
    uint8_t rotateRight(uint8_t value, int steps) const;
};

#endif