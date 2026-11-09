# vole_machine
# Vole Machine Simulator
### CS213 – Assignment 2 – Part 3

## Team Members

**Team:**
- Ahmed Abdulaziz Ahmed – 20260968
- Kamel Abdelsalam Kamel  – 20261624
- Sara Ashraf Mohamed  – 20261622
- Zyna Nasreldin Mabrouk  – 20260945

**Project Video:** 

---

## About the Project

This project is a simulation of the Vole Machine. It was developed as part of CS213 Assignment 2 – Part 3.

The simulator allows users to write or load instructions, execute them, and follow what happens inside the machine.

It also includes a graphical interface where users can see the memory, registers, program counter, and program output while the instructions are running.

### Main Features

- 16 registers, from R0 to RF, each holding 8 bits.
- 256 bytes of memory, with addresses from 00 to FF.
- A screen connected to memory address FF.
- 12 supported instructions.
- Each instruction is 16 bits (4 hexadecimal digits).
- Step-by-step execution or running the entire program.

---

## Project Files

The project is organized into the following folders and files:

- **code/** – Contains the C++ simulator and Qt graphical interface.
- **examples/** – Contains some sample programs to try.
- **snapshots/** – Contains screenshots of the application.
- **README.txt** – Project information and instructions.

The project source code is included, but the executable and Qt libraries are not included.

---

## 1. Requirements

Before running the project, make sure you have:

- A C++20 compiler (GCC, MinGW 64-bit, Clang, or MSVC).
- CMake 3.16 or newer.
- Qt 5 or Qt 6 with the Widgets module.

### Installing on Windows

We recommend using Qt Creator.

1. Download the Qt installer from https://www.qt.io/download.
2. Install Qt 6 with MinGW 64-bit.
3. Install Qt Creator.
4. Make sure CMake and MinGW are also installed.

### Installing on Ubuntu / Debian

```bash
sudo apt install build-essential cmake qt6-base-dev
```

### Installing on macOS

```bash
brew install cmake qt
```

---

## 2. Building and Running

There are two ways to run the application.

### Option 1: Using Qt Creator

1. Open Qt Creator.
2. Select File → Open File or Project.
3. Open `code/CMakeLists.txt`.
4. Select a suitable Kit, such as Desktop Qt 6 MinGW 64-bit.
5. Configure the project.
6. Press Run or use Ctrl+R.

The Vole Machine window should open.

### Option 2: Using the Terminal

Open a terminal in the main project folder and run:

```bash
cd code
cmake -S . -B build
cmake --build build
```

If CMake cannot find Qt, you may need to specify its installation path:

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/mingw_64"
```

To start the application on Windows:

```bash
build\VVMMSS.exe
```

On Linux or macOS:

```bash
./build/VVMMSS
```

If the application is missing Qt DLL files on Windows, add the Qt `bin` directory to PATH or use `windeployqt`.

---

## 3. Using the Simulator

### Running Your First Program

You can start by entering the following instructions in the Program box:

```text
2105
2203
5312
33FF
C000
```

Then:

1. Press **Load** to load the program.
2. Press **Run** to execute all instructions.

After execution, register R3 should contain `08`, and the Output area should display `8`.

You can also use **Step** to execute one instruction at a time.

### Available Buttons

| Button | Description |
|---|---|
| Load | Loads the instructions into memory. |
| Open file... | Opens a text file containing a program. |
| Step | Executes the next instruction. |
| Run | Executes the program until it stops. |
| Reset | Clears the registers, memory, and screen. |

### Information Displayed in the GUI

The application shows several parts of the machine:

- **Program Counter (PC):** Shows the address of the next instruction.
- **Instruction Register (IR):** Shows the last fetched instruction.
- **What it does:** Explains the current instruction in simple words.
- **Memory:** Displays all 256 memory locations in a 16 × 16 grid.
- **Registers:** Shows the values stored in R0 through RF.
- **Output:** Displays the values sent to the screen.
- **State:** Shows whether the machine is Idle, Ready, Running, Halted, or in Error.

The memory cells also use colors to help follow execution:

- Yellow indicates the next instruction.
- Pink indicates the instruction that was just executed.
- A pink ring around a register indicates that its value changed.

---

## 4. Program Format

Each instruction must be written on a separate line using exactly four hexadecimal characters.

For example:

```text
2105
2203
5312
33FF
C000
```

Comments can be added using a semicolon (`;`). Empty lines are also allowed.

If the program does not contain a HALT instruction (`C000`) at the end, the simulator adds one automatically.

The maximum program size is 128 instructions, including the final HALT instruction.

### Supported Instructions

| Code | Operation |
|---|---|
| 1RXY | Load register R from memory address XY |
| 2RXY | Load register R with value XY |
| 3RXY | Store register R at memory address XY |
| 40RS | Copy register R into register S |
| 5RST | Add registers S and T, then store the result in R |
| 6RST | Floating-point addition |
| 7RST | Bitwise OR |
| 8RST | Bitwise AND |
| 9RST | Bitwise XOR |
| AR0X | Rotate register R right by X bits |
| BRXY | Jump to address XY if R equals R0 |
| C000 | Stop the program |

Jump addresses are byte addresses and must be even numbers.

### Floating-Point Format

Instruction `6RST` uses a custom 8-bit floating-point format:

- 1 sign bit.
- 3 exponent bits (excess 4).
- 4 mantissa bits.

The value is calculated as:

`±0.mantissa × 2^(exponent - 4)`

Some examples:

| Hex | Value |
|---|---|
| 58 | 1.0 |
| 68 | 2.0 |
| 6C | 3.0 |
| 78 | 4.0 |
| E8 | -2.0 |

---

## 5. Example Programs

The `examples` folder contains programs that can be loaded directly into the simulator.

| File | Purpose |
|---|---|
| 01_add.txt | Adds 5 and 3 and stores the result in R3. |
| 02_screen.txt | Performs the same addition and displays the result. |
| 03_loop.txt | Uses a loop to count to 5. |
| 04_logic.txt | Performs an OR operation between F0 and 0F. |
| 05_float.txt | Adds 2.0 and 1.0 using floating-point instructions. |

---

## 6. Error Messages

The simulator displays messages when something goes wrong.

- **Invalid instruction:** The instruction does not contain exactly four valid hexadecimal characters.
- **Load a program first:** A program must be loaded before using Step or Run.
- **Stopped after 100,000 steps:** The program may contain an infinite loop.
- **Invalid opcode:** The instruction contains an unsupported operation.
- **Program counter ran past the end of memory:** The program reached the end of memory without stopping correctly.

---

## 7. Automated Tests

The project also includes automated tests for checking the simulator components without running the Qt interface.

To compile and run the tests:

```bash
cd code
g++ -std=c++20 tests.cpp ALU.cpp CPU.cpp Instruction.cpp Memory.cpp Register.cpp program.cpp Simulator.cpp -o tests
./tests
```

If all tests pass, the final output should show:

```text
Failed: 0
```

You can also build the tests using CMake:

```bash
cmake -S . -B build -DBUILD_TESTS=ON
cmake --build build
```

---

## 8. Source Code

The `code` folder contains the main project files:

| File | Purpose |
|---|---|
| main.cpp | Starts the application. |
| MainWindow.h / .cpp | Handles the graphical interface. |
| Simulator.h / .cpp | Connects the CPU and memory and controls execution. |
| CPU.h / .cpp | Handles fetching, decoding, and executing instructions. |
| ALU.h / .cpp | Performs arithmetic and logical operations. |
| Memory.h / .cpp | Manages memory and screen output. |
| Register.h / .cpp | Represents an 8-bit register. |
| Instruction.h / .cpp | Extracts instruction fields. |
| program.h / .cpp | Loads programs from text files. |
| tests.cpp | Contains automated tests. |

---

**CS213 – Assignment 2 – Part 2**
