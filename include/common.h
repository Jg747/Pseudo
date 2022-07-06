#ifndef common
#define common

#include <gui.h>
#include <compiler.h>

int menu(Gui *g, Compiler *d);
bool handleArgs(int argc, char* argv[], string &fileName, int &mode, bool &noGui, bool &easterEgg);

#endif