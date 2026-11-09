#pragma once
#include "Simulator.h"
#include "program.h"
class MainUI
{
private:
	Simulator* simulator;
	program* prog;

public:
	MainUI(Simulator* sim, program* p);
	bool loadProgramfromFile();
	void loadProgram();
	void ExecuteOneStep();
	void ExecuteAll();
	void displayMenu();
	void showMemory();
	void showRegisters();
	void run();
};


