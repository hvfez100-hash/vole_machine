#pragma once
#include<iostream>

class Simulator;
class MenuUI
{
private:
	Simulator* simulator;

public:
	MenuUI(Simulator* sim = nullptr);
	void loadProgramFromFile();
	void runAtOnce();
	void runStepByStep();
	void displayMenu();
	void showMemory();
	void showRegisters();
	void play();

};


