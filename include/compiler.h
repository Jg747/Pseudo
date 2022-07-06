#ifndef __compiler_h__
#define __compiler_h__

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <fstream>
#include <time.h>
#include <sstream>
#include <vector>
#include <stack>
#include <dirent.h>
#include <unistd.h>

#include "structures.h"
#include "errori.h"
#include "lang.h"
#include "calculator.h"
#include "phases.h"
#include "phase_1.h"
#include "phase_2.h"
#include "execution.h"
#include "translator.h"

#include "gui.h"

using std::cout;
using std::cin;
using std::string;
using std::ifstream;
using std::ofstream;
using std::fstream;
using std::vector;
using std::stack;
using std::to_string;

class Compiler {
	private:
		vector<Blocco> *blocchi;				//Blocchi di programma
		vector<variabile> *vars;				//Lista di variabili
		Errori *codici;							//Codici di errore [errori.h]
		string fileName;						//Nome del file da leggere
		int mode;								//Modalita
		bool mantieni;							//Mantieni o cancella puntatori

		Gui *g;
	
	public:
		Compiler(int mode, string nome, Gui *g) {
			this->vars = new vector<variabile>;
			this->blocchi = new vector<Blocco>;
			this->codici = new Errori(g);
			this->fileName = nome;
			this->mode = mode;
			this->mantieni = false;
			this->g = g;
		}

		Compiler(string nome, int mode, vector<variabile> *vars, Errori *codici, Gui *g) {
			this->blocchi = NULL;
			this->vars = vars;
			this->codici = codici;
			this->fileName = nome;
			this->mode = mode;
			this->mantieni = true;
			this->g = g;
		}
		
		~Compiler() {
			if (!mantieni) {
				if (vars != NULL) {
					delete vars;
				}
				if (blocchi != NULL) {
					delete blocchi;
				}
				if (codici != NULL) {
					delete codici;
				}
			} else {
				if (blocchi != NULL) {
					delete blocchi;
				}
			}
		}

		bool start() {
			
			Phase1 *fase1;
			Phase2 *fase2;
			Execution *esegui;
			Translator *traduci;

			size_t pos;
			bool isError = false;

			switch (this->mode) {
				case 0:
					//MODALITA' INTERPRETE (Correzione + Esecuzione pseudocodice)
					if (!Phases::correctFile(fileName)) {
						return false;
					}

					fase1 = new Phase1(fileName, vars, blocchi, codici);
					if (!fase1->passataUno()) {
						delete fase1;
						codici->printErrors();
						return false;
					}
					delete fase1;

					fase2 = new Phase2(fileName, vars, codici);
					if (!fase2->passataDue()) {
						delete fase2;
						codici->printErrors();
						return false;
					}
					delete fase2;

					/*cout<<"1\t\t2\t\t3\n";
					for (size_t i = 0; i < blocchi->size(); i++) {
						cout<<blocchi->at(i).filePos1<<"\t\t"<<blocchi->at(i).filePos2<<"\t\t"<<blocchi->at(i).filePos3<<"\n";
					}*/

					esegui = new Execution(fileName, vars, blocchi, codici, g);
					if (!esegui->execute()) {
						delete esegui;
						codici->printErrors();
						return false;
					}
					delete esegui;
					break;
				case 1:
					//MODALITA' CONVERSIONE (C++ -> Pseudo)
					if ((pos = fileName.rfind(".cpp")) == string::npos || pos+4 != fileName.length()) {
						if (g != NULL) {
							g->errorWindow();
							g->writeOnWindow(EXECUTE, Errori::printError(TRNSL_EXT));
							g->writeOnWindow(EXECUTE, "\n[premi un tasto per continuare]");
							g->getWindowChar(EXECUTE);
						} else {
							cout << Errori::printError(TRNSL_EXT) << "\n[premi un tasto per continuare]";
							getchar();
						}
						return false;
					}

					if (!Phases::correctFile(fileName)) {
						return false;
					}
					
					traduci = new Translator(0, fileName);
					if (!traduci->traduci()) {
						//cout<<Errori::printError(TRNSL_CPP_TXT);
						if (g != NULL) {
							g->errorWindow();
							g->writeOnWindow(EXECUTE, Errori::printError(TRNSL_CPP_TXT));
							g->writeOnWindow(EXECUTE, "\n[premi un tasto per continuare]");
							g->getWindowChar(EXECUTE);
						} else {
							cout << Errori::printError(TRNSL_CPP_TXT) << "\n[premi un tasto per continuare]";
							getchar();
						}
						return false;
					}
					delete traduci;

					fileName = fileName.substr(0, fileName.rfind(".")) + (fileName.find(".cpp") != string::npos ? ".txt" : ".cpp");

					fase1 = new Phase1(fileName, vars, blocchi, codici);
					if (!fase1->passataUno()) {
						delete fase1;
						codici->printErrors();
						return false;
					}
					delete fase1;

					fase2 = new Phase2(fileName, vars, codici);
					if (!fase2->passataDue()) {
						delete fase2;
						codici->printErrors();
						return false;
					}
					delete fase2;

					if (isError) {
						//cout<<Errori::printError(TRNSL_CPP_TXT);
						if (g != NULL) {
							g->errorWindow();
							g->writeOnWindow(EXECUTE, Errori::printError(TRNSL_CPP_TXT));
							g->writeOnWindow(EXECUTE, "\n[premi un tasto per continuare]");
							g->getWindowChar(EXECUTE);
						} else {
							cout << Errori::printError(TRNSL_CPP_TXT) << "\n[premi un tasto per continuare]";
							getchar();
						}
						return false;
					}
					break;
				case 2:
					//MODALITA' CONVERSIONE (Pseudo -> C++)
					if ((pos = fileName.rfind(".txt")) == string::npos || pos+4 != fileName.length()) {
						if (g != NULL) {
							g->errorWindow();
							g->writeOnWindow(EXECUTE, Errori::printError(TRNSL_EXT));
							g->writeOnWindow(EXECUTE, "\n[premi un tasto per continuare]");
							g->getWindowChar(EXECUTE);
						} else {
							cout << Errori::printError(TRNSL_EXT) << "\n[premi un tasto per continuare]";
							getchar();
						}
						return false;
					}

					if (!Phases::correctFile(fileName)) {
						return false;
					}

					fase1 = new Phase1(fileName, vars, blocchi, codici);
					if (!fase1->passataUno()) {
						delete fase1;
						codici->printErrors();
						return false;
					}
					delete fase1;

					fase2 = new Phase2(fileName, vars, codici);
					if (!fase2->passataDue()) {
						delete fase2;
						codici->printErrors();
						return false;
					}
					delete fase2;

					traduci = new Translator(1, fileName);
					if (!traduci->traduci()) {
						delete traduci;
						//cout<<Errori::printError(TRNSL_TXT_CPP);
						if (g != NULL) {
							g->errorWindow();
							g->writeOnWindow(EXECUTE, Errori::printError(TRNSL_TXT_CPP));
							g->writeOnWindow(EXECUTE, "\n[premi un tasto per continuare]");
							g->getWindowChar(EXECUTE);
						} else {
							cout << Errori::printError(TRNSL_TXT_CPP) << "\n[premi un tasto per continuare]";
							getchar();
						}
						return false;
					}
					delete traduci;
					break;
				default:
					//MODALITA' NON RICONOSCIUTA
					//cout<<"Modalita' del compilatore non riconosciuta!\n";
					if (g != NULL) {
						g->errorWindow();
						g->writeOnWindow(EXECUTE, "Modalita' del compilatore non riconosciuta!");
						g->writeOnWindow(EXECUTE, "\n[premi un tasto per continuare]");
						g->getWindowChar(EXECUTE);
					} else {
						cout << "Modalita' del compilatore non riconosciuta!\n[premi un tasto per continuare]";
						getchar();
					}
					return false;
					break;
			}

			return true;
			
		}
		
};

#endif
