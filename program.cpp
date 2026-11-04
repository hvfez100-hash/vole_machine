#include<program.h>
#include<fstream>
#include <cctype>
#include <string>
void program::addInstruction(uint16_t instruction) {
    instructions.push_back(instruction);
};
std::size_t program::getSize() const {
    return instructions.size();
};
 uint16_t program ::getInstruction (std::size_t index)
 {
    return instructions.at(index);
 }
 void program:: clear()
 {
    instructions.clear();
 };
 bool program::loadFromFile(std::string &filename)
 {
   std::ifstream file(filename);
   if (!file.is_open()) {
    return false;
}
std::string line;
while (std:: getline(file , line))
{
    if (line.empty()||line[0] == ';')
    continue;
if (line.length() != 4)
    return false; 
for (int i = 0 ; i < 4 ; i++)
{
    if ( !std::isxdigit(static_cast<unsigned char>(line[i])))
    return false;
}
unsigned long no = std::stoul(line , nullptr , 16);
instructions.push_back(static_cast<uint16_t>(no));
}
return true;


 };
 