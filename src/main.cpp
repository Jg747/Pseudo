/*
Compile command:
g++ -I cparse -std=c++20 cparse/builtin-features.o cparse/core-shunting-yard.o Pseudo.cpp

Utilizzo della "calcolatrice"
calcolatrice->setExpression(read, row);
calcolatrice->evaluate();							//Check se l'espressione è valida
calcolatrice->calculate();							//Calcolo del risultato	[da codificare]

MODALITA'
0 -> exec pseudo
1 -> C++ to Pseudo
2 -> Pseudo to C++

COMANDI
file.txt | file.cpp		FILE
-e						ESEGUI (solo nel caso di .txt)
-t						TRANSLATE (Pseudo -> C++ [.txt]) (C++ -> Pseudo [.cpp])
-nogui					ESEGUE SENZA GUI
-t ^ -e

-------- BUG --------

-------- INFO --------
[INFO] Possibilmente sistemare la conversione delle espressioni [spazi] (FromPseudo | ToPseudo)

-------- FEATURES DA AGGIUNGERE --------

-------- CONSIDERAZIONI FINALI --------
*/

#include <iostream>
#include <gui.h>
#include <lang.h>
#include <common.h>
#include <compiler.h>

#ifdef _WIN32
#include "cparse/shunting-yard.h"
#include "cparse/shunting-yard-exceptions.h"
#include "cparse/builtin-features.inc"
#else
#include "cparse/shunting-yard.h"
#include "cparse/shunting-yard-exceptions.h"
#endif

Gui *g;
Compiler *d;

int main(int argc, char* argv[]) {
    #ifdef _WIN32
	cparse_startup();
	#endif
	
	if (argc == 1) {
		g = new Gui();

		try {
			while(menu() != 0);
		} catch (const std::exception &e) {
			std::cout << TITLE << "\n\t\t\tQualcosa e' andato storto\n\n" << FOOTER;
		}
	} else {
		string fileName;
		int mode;
		bool noGui;
        bool easterEgg;

		if (handleArgs(argc, argv, fileName, mode, noGui, easterEgg)) {
			if (mode != -1) {
				if (mode == 0 || mode == 3) {
					if (!noGui) {
						g = new Gui();
                        g->setEasterEgg(easterEgg);
					} else {
						g = NULL;
					}
				} else {
					g = NULL;
				}
				d = new Compiler(mode, fileName, g);
				try {
					d->start();
				} catch (const std::exception &e) {
					std::cout << TITLE << "\n\t\t\tSomething went wrong!\n\n" << FOOTER;
				}
			}
		} else {
			std::cout << printHelp(false);
		}

	}

	if (g != NULL) {
		delete g;
	}

    return 0;
}