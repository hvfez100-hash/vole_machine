#include "ALU.h"
#include <iostream>
#include <cmath>
uint8_t ALU::addIntegers(uint8_t a, uint8_t b) {
   return a + b;
}
uint8_t ALU::addFloatingPoint(uint8_t a, uint8_t b) {
   uint8_t a_sign = (a >> 7) & 1;
   uint8_t b_sign = (b >> 7) & 1;

   uint8_t a_exponent = (a >> 3) & 15;
   uint8_t b_exponent = (b >> 3) & 15;

   uint8_t a_fraction = a & 7;
   uint8_t b_fraction = b & 7;

   double a_mantissa = 1.0 + a_fraction / 8.0;
   double b_mantissa = 1.0 + b_fraction / 8.0;

   int a_power = static_cast<int>(a_exponent) - 7;
   int b_power = static_cast<int>(b_exponent) - 7;

   double a_result = a_mantissa * std::pow(2.0, a_power);
   double b_result = b_mantissa * std::pow(2.0, b_power);

   if (a_sign == 1) {
      a_result = -a_result;
   }
   if (b_sign == 1) {
      b_result = -b_result;
   }

   double sum = a_result + b_result;

   uint8_t resultSign;
   if (sum < 0) {
      resultSign = 128;
   } else if (sum > 0){
      resultSign = 0;
   } else {
      return 0;
   }
   double magnitude = std::abs(sum);
   int power = static_cast<int>(std::floor(std::log2(magnitude)));
   double normal = magnitude / std::pow(2.0, power);
   int resultExponent = power + 7;
   uint8_t resultFraction = static_cast<uint8_t>((normal - 1.0) * 8.0 + 0.5);

   if (resultFraction == 8) {
      resultFraction = 0;
      resultExponent++;
   }

   return resultSign | (resultExponent << 3) | resultFraction;
}
uint8_t ALU::bitWiseAnd(uint8_t a, uint8_t b) {
   return a & b;
}
uint8_t ALU::bitWiseOr(uint8_t a, uint8_t b) {
   return a | b;
}
uint8_t ALU::bitWiseXor(uint8_t a, uint8_t b) {
   return a ^ b;
}
uint8_t ALU::rotateRight(uint8_t value, int steps) {
   steps = steps & 7;
   if (steps == 0) {
      return value;
   }
   return (value >> steps) | (value << (8 - steps));
}
