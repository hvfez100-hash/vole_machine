#pragma once

#include "Simulator.h"
class MainUI
{
private:
	Simulator* simulator;

public:
	MainUI(Simulator* sim = nullptr);
	void loadProgram();
	void ExecuteOneStep();
	void ExecuteAll();
	void displayMenu();
	void showMemory();
	void showRegisters();
	void run();

};


