cmake_minimum_required(VERSION 4.0)
project(finalvolemachine)

set(CMAKE_CXX_STANDARD 20)

add_executable(finalvolemachine
        main.cpp
        MainUI.cpp
        Simulator.cpp
        CPU.cpp
        ALU.cpp
        Instruction.cpp
        Memory.cpp
        Register.cpp
        program.cpp
)