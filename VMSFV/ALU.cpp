#include "ALU.h"
#include <cmath>
uint8_t ALU::addIntegers(uint8_t a, uint8_t b) const {
   return a + b;
}
uint8_t ALU::addFloatingPoint(uint8_t a, uint8_t b) const {
   uint8_t a_sign = (a >> 7) & 1;
   uint8_t b_sign = (b >> 7) & 1;

   uint8_t a_exponent = (a >> 4) & 7;
   uint8_t b_exponent = (b >> 4) & 7;

   uint8_t a_fraction = a & 15;
   uint8_t b_fraction = b & 15;

   double a_mantissa = a_fraction / 16.0;
   double b_mantissa = b_fraction / 16.0;

   int a_power = static_cast<int>(a_exponent) - 4;
   int b_power = static_cast<int>(b_exponent) - 4;

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
   int power = static_cast<int>(std::floor(std::log2(magnitude))) + 1;
   double normal = magnitude / std::pow(2.0, power);
   int resultExponent = power + 4;
   uint8_t resultFraction = static_cast<uint8_t>(normal * 16.0 + 0.5);

   if (resultFraction == 16) {
      resultFraction = 8;
      resultExponent++;
   }

   if (resultExponent > 7) {
      return resultSign | 0x7F;
   }
   if (resultExponent < 0) {
      return 0;
   }

   return resultSign | (resultExponent << 4) | resultFraction;
}
uint8_t ALU::bitWiseAnd(uint8_t a, uint8_t b) const {
   return a & b;
}
uint8_t ALU::bitWiseOr(uint8_t a, uint8_t b) const {
   return a | b;
}
uint8_t ALU::bitWiseXor(uint8_t a, uint8_t b) const {
   return a ^ b;
}
uint8_t ALU::rotateRight(uint8_t value, int steps) const {
   steps = steps & 7;
   if (steps == 0) {
      return value;
   }
   return (value >> steps) | (value << (8 - steps));
}
