#ifndef compiler
#define compiler

#include <gui.h>

#include <vector>

class Compiler {
	private:
		//vector<Blocco> *blocchi;				//Blocchi di programma
		//vector<variabile> *vars;				//Lista di variabili
		//Errori *codici;							//Codici di errore [errori.h]
		string fileName;						//Nome del file da leggere
		int mode;								//Modalita
		bool mantieni;							//Mantieni o cancella puntatori
		Gui *g;
	
	public:
        // Object handle
		Compiler(int mode, string nome, Gui *g);
		Compiler(string nome, int mode, vector<variabile> *vars, Errori *codici, Gui *g);
		~Compiler();

        // Start program
		bool start();
};

#endif