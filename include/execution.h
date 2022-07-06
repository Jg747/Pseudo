#ifndef __execution_h__
#define __execution_h__

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <fstream>
#include <vector>
#include <stack>
#include <limits>
#include <sstream>
#include <cmath>

#include "phases.h"
#include "structures.h"
#include "errori.h"
#include "lang.h"
#include "calculator.h"
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
using std::streampos;
using std::stringstream;
using std::isnan;

class Execution : public Phases {
	public:
		Execution(string fileName, vector<variabile> *vars, vector<Blocco> *blocchi, Errori *codici, Gui *g) {
			this->index = 0;
			this->blocchi = blocchi;
			this->vars = vars;
			this->codici = codici;
			this->fileName = fileName;
			this->calcolatrice = new Calcolatore(vars, codici);
			this->g = g;
			(this->ss).setf(std::ios::fixed);
			(this->ss).precision(0);
		}

		~Execution() {
			if (calcolatrice != NULL) {
				delete this->calcolatrice;
			}
		}

		bool execute() {
			//Esecuzione

			//Per annotarsi le righe da effettuare in funzione basta andare normalmente da pos1 a pos2 e dopo passare
			//alla funzione del ciclo da che parte a che parte arrivare e mettere la condizione di rientro del file
			//nel ciclo(?)
			//
			//Vedere file "Test calcolatore/test.cpp"

			if (!openFile()) {
				addError(ERR_FILE_OPEN);
				return false;
			}

			if (g != NULL) {
				g->executeWindow();
			}

			bool temp = true;
			for (row = 1; file && getline(file, read); row++) {
				if (!selectStructure(temp)) {
					return false;
				}
			}

			file.close();

			if (codici->isEmpty()) {
				if (g != NULL) {
					g->writeOnWindow(EXECUTE, string(END_EXEC_PGM) + string(PRESS_TO_CONTINUE));
					g->getWindowChar(EXECUTE);
				} else {
					cout << END_EXEC_PGM << PRESS_TO_CONTINUE;
					getchar();
				}
			}

			return codici->isEmpty();

		}

	private:
		vector<Blocco> *blocchi;
		int index;
		stringstream ss;
		Gui *g;

		bool selectStructure(bool &increment) {
			if ((pos = read.find(SE)) != string::npos && read.find(ESEGUI) == string::npos && read.find(FINE_SE) == string::npos) {
				if (!increment) {
					index++;
					//cout<<"inc SE - row: "<<row<<"\n";
					increment = true;
				}
				if (!executeSE()) {
					return false;
				}
				index++;
				//cout<<"inc SE (fine) - row: "<<row<<"\n";
			} else if ((pos = read.find(RIPETI)) != string::npos) {
				if (!increment) {
					index++;
					increment = true;
				}
				if (!executeFINOACHE()) {
					return false;
				}
				index++;
			} else if ((pos = read.find(FINTANTOCHE)) != string::npos) {
				if (!increment) {
					index++;
					//cout<<"inc WHILE - row: "<<row<<"\n";
					increment = true;
				}
				if (!executeFINTANTOCHE()) {
					return false;
				}
				index++;
				//cout<<"inc WHILE (fine) - row: "<<row<<"\n";
			} else if ((pos = read.find(LEGGI)) != string::npos) {
				if (!executeLEGGI()) {
					addError(RUN_INVALID_VALUE);
					return false;
				}
			} else if ((pos = read.find(SCRIVI)) != string::npos) {
				executeSCRIVI();
			} else if ((pos = read.find(ASSEGNAZIONE)) != string::npos) {
				if (!executeASSEGNAZIONE()) {
					return false;
				}
			}
			return true;
		}

		bool executeSE() {
			
			//cout<<index<<"\n";
			return executeStructure(blocchi->at(index), IF);

		}

		bool executeFINOACHE() {

			//cout<<index<<"\n";
			return executeStructure(blocchi->at(index), DOWHILE);

		}

		bool executeFINTANTOCHE() {

			//cout<<index<<"\n";
			return executeStructure(blocchi->at(index), WHILE);

		}

		bool executeStructure(Blocco b, blockType tipo) {

			streampos pos1 = b.filePos1;
			streampos pos2 = b.filePos2;
			streampos pos3 = b.filePos3;
			double result;
			int saveRow, saveIndex;
			bool increment;
			int i = 0;

			if (pos2 != -1 && tipo == IF) {
				tipo = IF_ELSE;
			}

			saveRow = row;
			saveIndex = index;
			increment = false;

			switch (tipo) {
				case WHILE:
					//Da pos1 a pos3 - CONDIZIONE INCLUSA
					//cout<<"WHILE\n";
					while (true) {
						row = saveRow;
						index = saveIndex;
						increment = false;
						file.seekg(pos1);
						getline(file, read);
						pos = read.find(INIZIO_EXP) + 1;
						read = read.substr(pos, read.rfind(FINE_EXP) - pos);

						calcolatrice->setExpression(read, row);
						result = calcolatrice->calculate();
						if (result == std::numeric_limits<double>::infinity()) {
							addError(EXP_ZERO_DIVISION);
							return false;
						}
						if (result) {
							for (getline(file, read), row++; file; getline(file, read), row++) {
								if (!selectStructure(increment)) {
									return false;
								}
								if (file.tellg() >= pos3) {
									row++;
									break;
								}
							}
						} else {
							row = saveRow;
							index = saveIndex;
							file.seekg(pos1);
							for (getline(file, read), row++; file; getline(file, read), row++) {
								if ((read.find(SE) != string::npos && read.find(ESEGUI) == string::npos && read.find(FINE_SE) == string::npos) || read.find(RIPETI) != string::npos || read.find(FINTANTOCHE) != string::npos) {
									index++;
								}
								if (file.tellg() >= pos3) {
									row++;
									break;
								}
							}
							index--;
							break;
						}
						i++;
						if (i > TRIGGER_INFINITE_LOOP_ERROR) {
							addError(RUN_INFINITE_LOOP);
							return false;
						}
					}
					break;
				case DOWHILE:
					//Da pos1 a pos3 CONDIZIONE ESCLUSA
					//cout<<"DO-WHILE\n";
					while (true) {
						row = saveRow;
						index = saveIndex;
						increment = false;
						file.seekg(pos1);
						for (getline(file, read); file && file.tellg() < pos3; getline(file, read), row++) {
							if (!selectStructure(increment)) {
								return false;
							}
						}
						
						pos = read.find(INIZIO_EXP) + 1;
						read = read.substr(pos, read.rfind(FINE_EXP) - pos);
						calcolatrice->setExpression(read, row);
						result = calcolatrice->calculate();
						if (result == std::numeric_limits<double>::infinity()) {
							addError(EXP_ZERO_DIVISION);
							return false;
						}
						if (result) {
							break;
						}
						i++;
						if (i > TRIGGER_INFINITE_LOOP_ERROR) {
							addError(RUN_INFINITE_LOOP);
							return false;
						}
					}
					row = saveRow;
					index = saveIndex;
					file.seekg(pos1);
					for (getline(file, read); file && file.tellg() < pos3; getline(file, read), row++) {
						if ((read.find(SE) != string::npos && read.find(ESEGUI) == string::npos && read.find(FINE_SE) == string::npos) || read.find(RIPETI) != string::npos || read.find(FINTANTOCHE) != string::npos) {
							index++;
						}
					}	
					break;
				case IF:
					//Check in pos1, true [pos1 -> pos3], false [pos1 -> jump pos3] - CONDIZIONE INCLUSA
					//cout<<"IF\n";
					row = saveRow;
					index = saveIndex;
					increment = false;
					file.seekg(pos1);
					getline(file, read);
					pos = read.find(INIZIO_EXP) + 1;
					read = read.substr(pos, read.rfind(FINE_EXP) - pos);
					calcolatrice->setExpression(read, row);
					result = calcolatrice->calculate();
					if (result == std::numeric_limits<double>::infinity()) {
						addError(EXP_ZERO_DIVISION);
						return false;
					}
					if (result) {
						for (getline(file, read), row++; file && file.tellg() < pos3; getline(file, read), row++) {
							if (!selectStructure(increment)) {
								return false;
							}
						}
					} else {
						row = saveRow;
						index = saveIndex;
						file.seekg(pos1);
						for (getline(file, read), row++; file && file.tellg() < pos3; getline(file, read), row++) {
							if ((read.find(SE) != string::npos && read.find(ESEGUI) == string::npos && read.find(FINE_SE) == string::npos) || read.find(RIPETI) != string::npos || read.find(FINTANTOCHE) != string::npos) {
								index++;
							}
						}
						index--;
					}
					break;
				case IF_ELSE:
					//Check in pos1, true [pos1; pos2 -> jump pos3], false [pos2; pos3] - CONDIZIONE INCLUSA
					//cout<<"IF-ELSE\n";
					row = saveRow;
					index = saveIndex;
					increment = false;
					file.seekg(pos1);
					getline(file, read);
					pos = read.find(INIZIO_EXP) + 1;
					read = read.substr(pos, read.rfind(FINE_EXP) - pos);
					calcolatrice->setExpression(read, row);
					result = calcolatrice->calculate();
					if (result == std::numeric_limits<double>::infinity()) {
						addError(EXP_ZERO_DIVISION);
						return false;
					}
					if (result) {
						for (getline(file, read), row++; file && file.tellg() < pos2; getline(file, read), row++) {
							if (!selectStructure(increment)) {
								return false;
							}
						}
						file.seekg(pos2);
						//row--;
						for (getline(file, read), row++; file && file.tellg() < pos3; getline(file, read), row++) {
							if ((read.find(SE) != string::npos && read.find(ESEGUI) == string::npos && read.find(FINE_SE) == string::npos) || read.find(RIPETI) != string::npos || read.find(FINTANTOCHE) != string::npos) {
								index++;
							}
						}
						index--;
					} else {
						for (getline(file, read), row++; file && file.tellg() < pos2; getline(file, read), row++) {
							if ((read.find(SE) != string::npos && read.find(ESEGUI) == string::npos && read.find(FINE_SE) == string::npos) || read.find(RIPETI) != string::npos || read.find(FINTANTOCHE) != string::npos) {
								index++;
							}
						}
						file.seekg(pos2);
						//row--;
						for (getline(file, read), row++; file && file.tellg() < pos3; getline(file, read), row++) {
							if (!selectStructure(increment)) {
								return false;
							}
						}
					}
					break;
			}

			return true;

		}

		bool executeASSEGNAZIONE() {
			
			//true se è a posto, false se EXP_ZERO_DIVISION
			double result;
			string temp;

			pos2 = read.find_first_not_of(" \t");
			temp = read.substr(pos2, read.find_first_of("< \t", pos2)-pos2); //temp e' la var che dovra contenere result

			read = read.substr(read.find(ASSEGNAZIONE) + string(ASSEGNAZIONE).length());

			calcolatrice->setExpression(read, row);

			result = calcolatrice->calculate();
			if (result == std::numeric_limits<double>::infinity()) {
				addError(EXP_ZERO_DIVISION);
				return false;
			}

			(vars->at(varIndex(temp))).valore = result;

			return true;
		
		}

		bool executeLEGGI() {
			
			//true se è a posto, false se RUN_INVALID_VALUE
			string input;
			string temp;

			read = read.substr(pos + string(LEGGI).length());

			while ((pos = read.find(",")) != string::npos) {
				temp = read.substr(0, pos);
				read = read.substr(pos + 1);
				pos = temp.find_first_not_of(" \t");
				temp = temp.substr(pos, temp.find_first_of(" \t", pos));
				
				//cin >> input;
				//while (cin.get() != '\n');
				if (g != NULL) {
					input = g->getInput(EXECUTE, 20);
				} else {
					getline(cin, input);
				}

				if (isNumber(input)) {
					(vars->at(varIndex(temp))).valore = atof(input.c_str());
				} else {
					return false;
				}
			}
			pos = read.find_first_not_of(" \t");
			pos2 = read.find_first_of(" \t", pos);
			if (pos2 != string::npos) {
				temp = read.substr(pos, pos2);
			} else {
				temp = read.substr(pos);
			}
			
			//cin >> input;
			//while (cin.get() != '\n');
			if (g != NULL) {
				input = g->getInput(EXECUTE, 20);
			} else {
				getline(cin, input);
			}

			if (isNumber(input)) {
				(vars->at(varIndex(temp))).valore = atof(input.c_str());
			} else {
				return false;
			}

			return true;

		}

		void executeSCRIVI() {

			read = read.substr(pos + string(SCRIVI).length());

			std::stringstream stringa(read);
			string temp;
			vector<string> tokens;
			size_t pos1;
			char carattere[2];
			strcpy(carattere, "");
			carattere[0] = 1;
			double valore;

			while (getline(stringa, temp, ',')) {
				tokens.push_back(temp);
			}

			for (size_t i = 0; i < tokens.size(); i++) {
				while ((pos1 = tokens[i].find("\\\"")) != string::npos) {
					tokens[i].replace(pos1, 2, carattere);
				}
				if ((pos1 = tokens[i].find("\"")) != string::npos) {
					if (tokens[i].find_first_not_of(" \t") == pos1) {
						tokens[i] = tokens[i].substr(pos1 + 1);
						if ((pos1 = tokens[i].rfind("\"")) != string::npos) {
							tokens[i] = tokens[i].substr(0, pos1);
						} else {
							tokens[i].push_back(',');
						}
					} else {
						tokens[i] = tokens[i].substr(0, pos2);
					}
					tokens[i] = replaceSpecialChars(tokens[i]);

					//cout << tokens[i];
					if (g != NULL) {
						g->writeOnWindow(EXECUTE, tokens[i]);
					} else {
						cout << tokens[i];
					}

				} else {

					tokens[i] = tokens[i].substr(tokens[i].find_first_not_of(" \t"));
					pos1 = tokens[i].find_first_of(" \t");
					if (pos1 != string::npos) {
						tokens[i] = tokens[i].substr(0, pos1);
					}

					//cout << (vars->at(varIndex(tokens[i]))).valore;
					valore = (vars->at(varIndex(tokens[i]))).valore;
					if (fmod(valore, floor(valore)) != 0 && !isnan(fmod(valore, floor(valore)))) {
						ss.precision(2);
						ss << valore;
						temp = ss.str();
						ss.precision(0);
					} else {
						ss << valore;
						temp = ss.str();
					}
					ss.str("");

					if (g != NULL) {
						g->writeOnWindow(EXECUTE, temp);
					} else {
						cout << temp;
					}

				}
			}

		}

		string replaceSpecialChars(string str) {

			for (size_t i = 0; str[i]; i++) {
				if (str[i] == '\\') {
					i++;
					switch (str[i]) {
						case 'a':
							str.replace(i-1, 2, "\a");
							break;
						case 'b':
							str.replace(i-1, 2, "\b");
							break;
						case 'n':
							str.replace(i-1, 2, "\n");
							break;
						case 'r':
							str.replace(i-1, 2, "\r");
							break;
						case 't':
							str.replace(i-1, 2, "\t");
							break;
						case '\\':
							str.replace(i-1, 2, "\\");
							break;
						case '\'':
							str.replace(i-1, 2, "\'");
							break;
						case '\"':
							str.replace(i-1, 2, "\"");
							break;
						case '\?':
							str.replace(i-1, 2, "\?");
							break;
						default:
							break;
					}
					i--;
				} else if (str[i] == 1) {
					str.replace(i, 1, "\"");
				}
			}

			return str;

		}

};

#endif