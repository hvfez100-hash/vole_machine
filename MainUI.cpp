#include "MainUI.h"
#include "Simulator.h"
#include <iomanip>
#include<iostream>
using namespace std;

MainUI::MainUI(Simulator* sim) : simulator(sim) {}

void MainUI::displayMenu() {
	cout << "\n||=====Menu=====||\n";
	cout << "1. Load Program\n";
	cout << "2. Execute One Step\n";
	cout << "3. Execute All\n";
	cout << "4. Display Memory\n";
	cout << "5. Display Register\n";
	cout << "6. Reset\n";
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
			cout << "Invalid choice. Please enter a number." << endl;
			continue;
		}		switch (choice) {
			case 1:
				loadProgram();
				break;
			case 2:
				ExecuteOneStep();
				break;
			case 3:
				ExecuteAll();
				cout << "Program Executed." << "\n------------------------------\n" << endl;
				break;
			case 4:
				showMemory();
				break;
			case 5:
				showRegisters();
				break;
			case 6:
				simulator->reset();
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

