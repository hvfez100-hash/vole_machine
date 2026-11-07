#include"program.h"
#include<fstream>
#include <cctype>
#include <string>
#include<vector>
#include<utility>
void program::addInstruction(uint16_t instruction) {
    instructions.push_back(instruction);
};
std::size_t program::getSize() const {
    return instructions.size();
};
 uint16_t program::getInstruction (std::size_t index) const
 {
    return instructions.at(index);
 }
 void program:: clear()
 {
    instructions.clear();
 };
bool program::loadFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    std::string line;
    std:: vector<uint16_t> testcase;
    while (std:: getline(file , line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (line.find_first_not_of(" \t") == std::string::npos)
        continue;

if (line[line.find_first_not_of(" \t")] == ';')
       continue;
        if (line.length() != 4)
            return false;
        for (int i = 0 ; i < 4 ; i++)
        {
            if ( !std::isxdigit(static_cast<unsigned char>(line[i])))
                return false;
        }
        unsigned long no = std::stoul(line , nullptr , 16);
        testcase.push_back(static_cast<uint16_t>(no));
    }
    instructions = std:: move(testcase);
    return true;


};
