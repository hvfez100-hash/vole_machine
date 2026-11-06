#ifndef PROGRAM_H
#define PROGRAM_H
#include<vector>
#include<cstdint>
#include<string>
#include <cstddef>
class program{
    private :
    std::vector<uint16_t> instructions;
    public :
 void addInstruction (	uint16_t instruction );
 uint16_t getInstruction (std::size_t index);
 std::size_t getSize() const;
 bool loadFromFile(std::string &filename);
 void clear();
};
#endif