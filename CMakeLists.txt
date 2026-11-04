cmake_minimum_required(VERSION 3.20)
project(Vole_Machine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Vole_Machine
        ALU/src/ALU.cpp
        Instruction/src/Instruction.cpp
        Register/src/Register.cpp
)

target_include_directories(Vole_Machine PUBLIC
        ALU/include
        Instruction/include
        Register/include
)
