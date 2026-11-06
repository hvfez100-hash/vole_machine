#include "MenuUI.h"
#include "Simulator.h"
#include<iostream>
using namespace std;


MenuUI::MenuUI(Simulator* sim) : simulator(sim) {}

void MenuUI::displayMenu() {
	cout << "\n||=====Menu=====||" <<endl<< endl;
	cout << "1. Load Program From File" << endl;
	cout << "2. Run Program at once" << endl;
	cout << "3. Run Program Step By Step" << endl;
	cout << "4. Dispalay Memoroy" << endl;
	cout << "5. Dispalay Register" << endl;
	cout << "6. Exit" << endl;
		cout<<"Enter choice: ";
}
void MenuUI::showMemory() {
	cout << "Memory:" << endl;
	for (int i = 0;i < 256;i++) {
		cout << i << " ";
		if (i % 16 == 0 && i!=0)
			cout<< endl<<"__________________________________________\n";
		
	}
	cout << "\n::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n" << endl;

}

void MenuUI::showRegisters() {
	cout << "Registers:" << endl<<endl;
	for (int i = 0; i < 16; i++) {
		cout << "R" << i << ":" << "0"<<i<<" ";
		if ((i + 1) % 4 == 0)
			cout << endl << "\n---------------------------------------------------\n";
	}
	cout << "\n::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n" << endl;

}

void MenuUI::play(){
	int choice=0;
	while (choice != 4) {
		displayMenu();
		cin >> choice;
		switch (choice) {
		case 1:
			loadProgramFromFile();
			break;
		case 2:
			runAtOnce();
			break;
		case 3:
			runStepByStep();
			cout << "Playing simulation..." << "\n------------------------------\n" << endl;
			break;
		case 4:
			showMemory();
			break;
		case 5:
			showRegisters();
			break;
		case 6:
			cout << "Goodbye!" << endl;
			break;
		default:
			cout << "Invalid choice. Please try again." << endl;
		}
	}

}
void MenuUI::loadProgramFromFile() {
	simulator->loadprogram({ 0x1234, 0x5678, 0x9ABC, 0xDEF0 });
}
void MenuUI::runAtOnce() {
	simulator->runAll();
}
void MenuUI::runStepByStep() {
	simulator->runstep();
}
int main() {
	Simulator* sim = nullptr; 
	MenuUI menu(sim);
	menu.play();
	return 0;
}
