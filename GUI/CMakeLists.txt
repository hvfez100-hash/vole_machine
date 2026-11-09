cmake_minimum_required(VERSION 3.16)
project(VVMMSS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_executable(VVMMSS
        main.cpp
        MainWindow.cpp
        MainWindow.h
        Simulator.cpp
        CPU.cpp
        ALU.cpp
        Instruction.cpp
        Memory.cpp
        Register.cpp
        program.cpp
        ALU.h
        CPU.h
        Instruction.h
        Memory.h
        program.h
        Register.h
        Simulator.h
)

target_link_libraries(VVMMSS PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
