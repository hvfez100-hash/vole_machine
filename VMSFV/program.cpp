#include"program.h"
#include<fstream>
#include <cctype>
#include <string>
#include<vector>
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
    if (!file.is_open())
        return false;

    std::string line;
    std::vector<uint16_t> loaded;
    bool first = true;

    while (std::getline(file, line)) {
        if (first && line.size() >= 3 &&
            (unsigned char)line[0] == 0xEF && (unsigned char)line[1] == 0xBB &&
            (unsigned char)line[2] == 0xBF)
            line.erase(0, 3);
        first = false;

        size_t sc = line.find(';');
        if (sc != std::string::npos)
            line.erase(sc);

        size_t b = line.find_first_not_of(" \t\r\n");
        if (b == std::string::npos)
            continue;
        size_t e = line.find_last_not_of(" \t\r\n");
        line = line.substr(b, e - b + 1);

        if (line.length() != 4) {
            return false;
        }
        for (char c : line) {
            if (!std::isxdigit(static_cast<unsigned char>(c))) {
                return false;
            }
        }
        loaded.push_back(static_cast<uint16_t>(std::stoul(line, nullptr, 16)));
    }
    if (!instructions.empty() && instructions.back() == 0xC000) {
        instructions.pop_back();
    }

    instructions.insert(instructions.end(), loaded.begin(), loaded.end());
    return true;
}