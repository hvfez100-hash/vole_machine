#include "MainUI.h"
#include "Simulator.h"

int main()
{
    Simulator sim;
    MainUI menu(&sim);

    menu.run();

    return 0;
}