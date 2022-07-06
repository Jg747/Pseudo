#include <common.h>
#include <gui.h>
#include <lang.h>
#include <compiler.h>

#include <fstream>
#include <unistd.h>
#include <vector>

int menu(Gui *g, Compiler *d) {
	if (g != NULL) {
        std::string x, nome;
		std::ifstream test;
	
		g->selectWindow();
		x = g->getInput(SELECT, 20);
		
		if (x.length() == 1) {
			switch (x[0]) {
				case '1':
					do {
						g->fileWindow();
						nome = g->getInput(SELECT, 255);
						if (nome.length() == 1 && nome[0] == '0') {
							return -1;
						}
						test.open(nome);
						if (!test.is_open()) {
							g->writeInvalid(SELECT, INVALID_INPUT, nome);
							sleep(2);
						}
					} while (!test.is_open());

					d = new Compiler(0 + (g->getEasterEgg() ? 3 : 0), nome, g);
					d->start();
					delete d;
					break;
				case '2':
					do {
						g->fileWindow();
						nome = g->getInput(SELECT, 255);
						if (nome.length() == 1 && nome[0] == '0') {
							return -1;
						}
						test.open(nome);
						if (!test.is_open()) {
							g->writeInvalid(SELECT, INVALID_INPUT, nome);
							sleep(2);
						}
					} while (!test.is_open());

					d = new Compiler(1 + (g->getEasterEgg() ? 3 : 0), nome, g);
					d->start();
					delete d;
					break;
				case '3':
					do {
						g->fileWindow();
						nome = g->getInput(SELECT, 255);
						if (nome.length() == 1 && nome[0] == '0') {
							return -1;
						}
						test.open(nome);
						if (!test.is_open()) {
							g->writeInvalid(SELECT, INVALID_INPUT, nome);
							sleep(2);
						}
					} while (!test.is_open());
					d = new Compiler(2 + (g->getEasterEgg() ? 3 : 0), nome, g);
					d->start();
					delete d;
					break;
				case '4':
					g->helpWindow(printHelp(g->getEasterEgg()) + string(PRESS_TO_CONTINUE));
					break;
				case '0':
					return 0;
					break;
				default:
					g->writeInvalid(SELECT, INVALID_INPUT, x);
					sleep(2);
					return -1;
					break;
			}
		} else if (x == "paolo") {
			g->setEasterEgg(true);
		} else {
			g->writeInvalid(SELECT, INVALID_INPUT, x);
			sleep(2);
		}

		return -1;
    }
	return 0;
}

bool handleArgs(int argc, char* argv[], string &fileName, int &mode, bool &noGui, bool &easterEgg) {
	
	std::string arg;

	if (argc > 7) {
		return false;
	}

	/*
	Modes
	0 -> exec pseudo
	1 -> C++ to Pseudo
	2 -> Pseudo to C++

	COMANDI
	-f <file>				File name
	-e						Execute (can't go with -t)
	-t <arg>				Translate (can't go with -e) Arg: ctp | ptc
	-nogui					No gui (ncurses window) execution
	-paolo					EasterEgg mode
	*/

	bool execute = false;
	bool translate = false;
	bool file = false;
	noGui = false;
	easterEgg = false;

	mode = -1;

	for (int i = 1; i < argc; i++) {
		arg = string(argv[i]);
		if (arg == "-e") {
			execute = true;
			mode = 0;
			if (translate) {
				return false;
			}
		} else if (arg == "-t") {
			translate = true;
			if (execute) {
				return false;
			}
			i++;
			arg = string(argv[i]);
			if (arg == "ctp") {
				mode = 1;
			} else if (arg == "ptc") {
				mode = 2;
			} else {
				return false;
			}
		} else if (arg == "-f") {
			file = true;
			i++;
			fileName = string(argv[i]);
		} else if (arg == "-nogui") {
			noGui = true;
		} else if (arg == "-help") {
			return false;
		} else if (arg == "-paolo") {
			easterEgg = true;
		} else {
			return false;
		}
	}

	if (mode != -1 && easterEgg) {
		mode += 3;
	}

	if (execute && mode != 0) {
		return false;
	}

	return true;
}