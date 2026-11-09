#include "MainUI.h"
#include "Simulator.h"
#include "program.h"

int main()
{
    Simulator sim;
    program prog;
    MainUI menu(&sim, &prog);

    menu.run();

    return 0;
}