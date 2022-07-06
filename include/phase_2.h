#ifndef __phase_2_h__
#define __phase_2_h__

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

#include "phases.h"
#include "structures.h"
#include "errori.h"
#include "lang.h"
#include "calculator.h"

using std::cout;
using std::cin;
using std::string;
using std::ifstream;
using std::ofstream;
using std::fstream;
using std::vector;
using std::stack;
using std::to_string;

class Phase2 : public Phases {
	public:
		Phase2(string fileName, vector<variabile> *vars, Errori *codici) {
			this->vars = vars;
			this->codici = codici;
			this->fileName = fileName;
			this->calcolatrice = new Calcolatore(vars, codici);
		}

		~Phase2() {
			delete this->calcolatrice;
		}

		bool passataDue() {
			//Controllo delle espressioni (assegnazioni, condizioni, valori)
			if (!openFile()) {
				addError(ERR_FILE_OPEN);
				return false;
			}

			for (row = 1; file && getline(file, read); row++) {
				if ((pos = read.find(ASSEGNAZIONE)) != string::npos) {
					checkASSEGNAZIONE();
				} else if (((pos = read.find(SE)) != string::npos && read.find(FINE_SE) == string::npos && read.find(ESEGUI) == string::npos) || (pos = read.find(FINTANTOCHE)) != string::npos || (pos = read.find(FINO_A_CHE)) != string::npos) {
					checkCONDIZIONE();
				} else if ((pos = read.find(LEGGI)) != string::npos) {
					checkLEGGI();
				} else if ((pos = read.find(SCRIVI)) != string::npos) {
					checkSCRIVI();
				}
			}

			file.close();

			return codici->isEmpty();

		}

	private:
		void checkASSEGNAZIONE() {
			
			string temp;

			pos2 = read.find_first_not_of(" \t");
			temp = read.substr(pos2, read.find_first_of("< \t", pos2)-pos2);
			if (!varsContains(temp)) {
				addVariable(temp);
			}

			read = read.substr(read.find(ASSEGNAZIONE) + string(ASSEGNAZIONE).length());

			calcolatrice->setExpression(read, row);
			
			if (!calcolatrice->evaluate()) {
				addError(SYN_EXP_WRONG);
			}
			
		}

		void checkLEGGI() {
			string temp;

			read = read.substr(pos + string(LEGGI).length());

			while ((pos = read.find(",")) != string::npos) {
				temp = read.substr(0, pos);
				read = read.substr(pos + 1);
				trim(temp);
				if (!varsContains(temp)) {
					addVariable(temp);
				}
			}
			trim(read);
			if (!varsContains(read)) {
				addVariable(read);
			}
		}

		void checkSCRIVI() {
			read = read.substr(pos + string(SCRIVI).length());

			for (string temp = nextWordScrivi(read); ; temp = nextWordScrivi(read)) {
				if (temp.find("\"") == string::npos) {
					if (!varsContains(temp)) {
						addError(VAR_NOT_EXIST);
					}
				}
				if (read.empty()) {
					break;
				}
			}

		}

		void checkCONDIZIONE() {

			pos = read.find(INIZIO_EXP);
			read = read.substr(pos+1);
			pos2 = read.rfind(FINE_EXP);
			read = read.substr(0, pos2);

			read = read.substr(read.find_first_not_of(" \t"));
			
			calcolatrice->setExpression(read, row);

			if (!calcolatrice->evaluate()) {
				addError(SYN_EXP_WRONG);
			}

		}

};

#endif
