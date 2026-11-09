#include "MainUI.h"
#include"program.h"
#include "Simulator.h"
#include <iomanip>
#include<iostream>
#include <filesystem>
using namespace std;

MainUI::MainUI(Simulator* sim, program* p) : simulator(sim), prog(p) {}
bool MainUI::loadProgramfromFile() {
	string filename;
	cout << "Enter the filename to load: ";
	cin >> ws;
	getline(cin, filename);
	if (!prog->loadFromFile(filename)) {
		cout << "Could not load \"" << filename << "\".\n"
			 << "Looking in: " << std::filesystem::current_path() << "\n"
			 << "Check the name, and that each line is 4 hex digits.\n";
		return false;
	}
	vector<uint16_t> instructions;
	for (size_t i = 0; i < prog->getSize(); i++) {
		instructions.push_back(prog->getInstruction(i));
	}
	if (instructions.empty() || instructions.back() != 0xC000) {
		instructions.push_back(0xC000);
	}
	simulator->reset();
	simulator->loadprogram(instructions);
	cout << "Program loaded successfully! (" << prog->getSize() << " instructions)\n";
	return true;
}
void MainUI::displayMenu() {
	cout << "\n||=====Menu=====||\n";
	cout << "1. Load Program from File\n";
	cout << "2. Load Program\n";
	cout << "3. Execute One Step\n";
	cout << "4. Execute All\n";
	cout << "5. Display Memory\n";
	cout << "6. Display Register\n";
	cout << "7. Reset Simulator\n";
	cout << "0. Exit\n";
	cout<<"Enter choice: ";
}
void MainUI::showMemory() {
	cout << "Memory:" << endl;
	for (int i = 0; i < 256; i++) {
		cout << hex << i << ":"
			<< setw(2) << setfill('0') << (int)simulator->readMemory(i) << "  ";

		if ((i + 1) % 16 == 0) {
			cout << endl;
		}
	}
	cout << dec;
	cout << "\n::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n" << endl;

}

void MainUI::showRegisters() {
		cout << "Registers:" << endl;
		for (int i = 0; i < 16; i++) {
			cout << "R" << dec << i << ": "
				 << hex << uppercase << setw(2) << setfill('0')
				 << (int)simulator->getRegister(i) << "\t";

			if ((i + 1) % 4 == 0)
				cout << endl;
		}
		cout << dec << nouppercase << setfill(' ');
		cout << "\n::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n" << endl;
	}


void MainUI::loadProgram() {
	vector<uint16_t> instructions;
	string input;
	cout << "Enter instructions (C000 to stop):" << endl;
	while (cin >> input && input != "C000" && input != "c000") {
		try {
			instructions.push_back(stoul(input, nullptr, 16));
		}
		catch (...) {
			cout << "Invalid hex instruction: " << input << ". Please try again." << endl;
		}

	}
	instructions.push_back(0xC000);
	simulator->loadprogram(instructions);
	cout << "Program loaded successfully!" << endl;
}
void MainUI::ExecuteOneStep() {
	simulator->runstep();
}
void MainUI::ExecuteAll() {
	simulator->runAll();
}
void MainUI::run(){
	int choice=0;
	do {
		displayMenu();
		if (!(cin >> choice)) {
			cin.clear(); 
			cin.ignore(10000, '\n'); 
			cin.clear();
			cin.ignore(10000, '\n');
			cout << "Invalid choice. Please enter a number." << endl;
			continue;
		}		switch (choice) {
			case 1:
				loadProgramfromFile();
				break;
			case 2:
				loadProgram();
				break;
			case 3:
				ExecuteOneStep();
				break;
			case 4:
				ExecuteAll();
				cout << "Program Executed." << "\n------------------------------\n" << endl;
				break;
			case 5:
				showMemory();
				break;
			case 6:
				showRegisters();
				break;
			case 7:
				simulator->reset();
				prog->clear();
				cout << "Simulator reset.\n";
				break;
			case 0:
				cout << "Goodbye!" << endl;
				break;
			default:
				cout << "Invalid choice. Please try again." << endl;
		}
	} while (choice != 0);

}

