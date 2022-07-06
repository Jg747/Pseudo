#ifndef __errori_h__
#define __errori_h__

#include <iostream>
#include <vector>
#include <string>

#include "lang.h"
#include "gui.h"

using std::string;
using std::to_string;
using std::cout;
using std::vector;

enum errorCode {
	
	//File
	ERR_FILE_OPEN,

	//Generale
	GEN_EXTRA_CHAR,
	GEN_DID_NOT_START,
	
	//Symbol
	SYM_NOT_EXIST,
	SYM_SYNTAX,
	
	//Variabili
	VAR_NOT_EXIST,
	VAR_NAME_WRONG,
	
	//Espressioni
	EXP_MISSING_APERTA,
	EXP_MISSING_CHIUSA,
	EXP_MISMATCH_PARENTESI,
	SYN_EXP_WRONG,
	EXP_ZERO_DIVISION,
	EXP_UNKNOWN_EXCEPTION,
	EXP_MISSING_CONDITION,
	
	//Syntax
	SYN_BLK_MISSING_START,
	SYN_BLK_MISSING_END,
	SYN_INCORRECT_STRUCTURE,

	//Run time
	RUN_DIVISION_ZERO,
	RUN_INVALID_VALUE,
	RUN_INFINITE_LOOP,

	//Traduzione
	TRNSL_CPP_TXT,
	TRNSL_TXT_CPP,
	TRNSL_EXT,
	
};

struct erroreFull {
	enum errorCode code;
	int row;
};

class Errori {
	private:
		vector<erroreFull> *arr;
		Gui *g;
	public:
		Errori(Gui *g) {
			this->arr = new vector<erroreFull>;
			this->g = g;
		}
		
		~Errori() {
			delete arr;
		}
		
		void add(erroreFull elem) {
			arr->push_back(elem);
		}
		
		void printErrors() {
			if (g != NULL) {
				g->errorWindow();
				erroreFull var;
				string temp;
				for (size_t i = 0; i < arr->size(); i++) {
					var = arr->at(i);
					temp = "Line " + to_string(var.row) + "\t\t" + printError(var.code);
					g->writeOnWindow(EXECUTE, temp);
					//cout<<"\t\tLine "<<var.row<<"\t\t"<<printError(var.code);
				}
				g->writeOnWindow(EXECUTE, "\n[premi un tasto per continuare]");
				g->getWindowChar(EXECUTE);
			} else {
				erroreFull var;
				string temp;
				for (size_t i = 0; i < arr->size(); i++) {
					var = arr->at(i);
					temp = "Line " + to_string(var.row) + "\t\t" + printError(var.code);
					cout << temp;
					//cout<<"\t\tLine "<<var.row<<"\t\t"<<printError(var.code);
				}
				cout << "\n[premi un tasto per continuare]";
				getchar();
			}
		}
		
		bool isEmpty() {
			return arr->empty();
		}

		static string printError(errorCode error) {
			switch (error) {
				case GEN_EXTRA_CHAR:
					return "[Trovato carattere extra assieme alla parola chiave]\n";
					break;
				case SYN_BLK_MISSING_START:
					return "[" + string(INIZIO) + " mancante]\n";
					break;
				case SYN_BLK_MISSING_END:
					return "[" + string(FINE) + " mancante]\n";
					break;
				case SYM_NOT_EXIST:
					return "[Simbolo non riconosciuto]\n";
					break;
				case SYM_SYNTAX:
					return "[Sintassi non corretta]\n";
					break;
				case VAR_NOT_EXIST:
					return "[Variabile non dichiarata]\n";
					break;
				case EXP_MISSING_APERTA:
					return "[\"(\" mancante]\n";
					break;
				case EXP_MISSING_CHIUSA:
					return "[\")\" mancante]\n";
					break;
				case SYN_EXP_WRONG:
					return "[Espressione non corretta]\n";
					break;
				case SYN_INCORRECT_STRUCTURE:
					return "[Sintassi della struttura non corretta]\n";
					break;
				case EXP_MISMATCH_PARENTESI:
					return "[Parentesi dell'espressione non corrette]\n";
					break;
				case VAR_NAME_WRONG:
					return "[Nome della variabile contenente caratteri non corretti]\n";
					break;
				case EXP_ZERO_DIVISION:
					return "[Divisione per 0 non eseguibile]\n";
					break;
				case ERR_FILE_OPEN:
					return "[Errore nell'apertura del file]\n";
					break;
				case RUN_DIVISION_ZERO:
					return "[Divisione per 0 non eseguibile]\n";
					break;
				case RUN_INVALID_VALUE:
					return "[Valore invalido inserito in INPUT]\n";
					break;
				case RUN_INFINITE_LOOP:
					return "[Ciclo infinito rilevato]\n";
					break;
				case EXP_UNKNOWN_EXCEPTION:
					return "[Errore nella produzione del risultato - EXP_UNKNOWN_EXCEPTION]\n";
					break;
				case EXP_MISSING_CONDITION:
					return "[Condizione non fornita]\n";
					break;
				case TRNSL_CPP_TXT:
					return "[Errore nella traduzione (C++ -> Pseudo), errore nel file C++?]\n";
					break;
				case TRNSL_TXT_CPP:
					return "[Errore nella traduzione (Pseudo -> C++), errore nel file Pseudo?]\n";
					break;
				case TRNSL_EXT:
					return "[TRANSLATOR] Estensione del file non valida!\n";
					break;
				case GEN_DID_NOT_START:
					return "[Trovati simboli prima di aver dato \"" + string(INIZIO) + "\"]";
					break;
			}
			return "";
		}

};

#endif
