#ifndef __phase_1_h__
#define __phase_1_h__

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <fstream>
#include <vector>
#include <stack>
#include <cstring>

#include "phases.h"
#include "structures.h"
#include "errori.h"
#include "lang.h"
#include "calculator.h"

#define OFFSET read.length()

using std::cout;
using std::cin;
using std::string;
using std::ifstream;
using std::ofstream;
using std::fstream;
using std::vector;
using std::stack;
using std::to_string;

class Phase1 : public Phases {
	public:
		Phase1(string fileName, vector<variabile> *vars, vector<Blocco> *blocchi, Errori *codici) {
			this->vars = vars;
			this->codici = codici;
			this->fileName = fileName;
			this->calcolatrice = new Calcolatore(vars, codici);
			this->blocchi = blocchi;
			this->index = 0;
		}

		~Phase1() {
			delete this->calcolatrice;
		}

		bool passataUno() {
			//Parentesi

			if (!openFile()) {
				addError(ERR_FILE_OPEN);
				return false;
			}

			for (row = 1; file && getline(file, read); row++) {
				if (read.find_first_not_of(" \t") != string::npos) {
					if (read.find(INIZIO) == string::npos) {
						addError(GEN_DID_NOT_START);
						return false;
					}
					graffe.push(_NO);
					row++;
					break;
				}
			}
			
			for (; file && getline(file, read); row++) {
				selectStructure();
			}
			
			graffe.pop();

			if (!graffe.empty()) {
				addError(SYN_BLK_MISSING_END);
			}

			file.close();

			return codici->isEmpty();
			
		}

	private:
		stack<enum stato> graffe;
		vector<Blocco> *blocchi;
		size_t index;

		void selectStructure() {
			checkBrackets();

			if ((pos = read.find(ASSEGNAZIONE)) != string::npos) {
				checkASSEGNAZIONE();
			} else if ((pos = read.find(LEGGI)) != string::npos) {
				checkLEGGI();
			} else if ((pos = read.find(SCRIVI)) != string::npos) {
				checkSCRIVI();
			} else if ((pos = read.find(SE)) != string::npos && read.find(ESEGUI) == string::npos && read.find(FINE_SE) == string::npos) {
				checkIF();
			} else if ((pos = read.find(RIPETI)) != string::npos) {
				checkFINOACHE();
			} else if ((pos = read.find(FINTANTOCHE)) != string::npos) {
				checkFINTANTOCHE();
			} else if (read.find(FINO_A_CHE) == string::npos && read.find(RIPETI) == string::npos && read.find(FINTANTOCHE) == string::npos && read.find(FINE_SE) == string::npos && read.find(ALTRIMENTI) == string::npos && read.find(ALLORA) == string::npos && read.find(SE) == string::npos && read.find(LEGGI) == string::npos && read.find(SCRIVI) == string::npos && read.find(INIZIO) == string::npos && read.find(FINE) == string::npos && read.find_first_not_of(" \t") != string::npos && read.find(ASSEGNAZIONE) == string::npos) {
				addError(SYM_NOT_EXIST);
			}
		}

		bool checkBrackets() {
			if ((pos = read.find(INIZIO)) != string::npos) {
				if (checkExtraChars(read, INIZIO, pos)) {
					if (graffe.empty()) {	
						addError(SYN_BLK_MISSING_END);
					}
				} else {
					addError(GEN_EXTRA_CHAR);
				}
				return true;
			} else if ((pos = read.find(FINE)) != string::npos && read.find(FINE_SE) == string::npos) {
				if (checkExtraChars(read, FINE, pos)) {
					if (graffe.empty()) {
						addError(SYN_BLK_MISSING_START);
					}
				} else {
					addError(GEN_EXTRA_CHAR);
				}
				return true;
			}
			return false;
		}

		void addGraffaPosition(enum filePosition pos, size_t offset) {
			Blocco graffa;

			switch (pos) {
				case POS1:
					graffa.filePos1 = file.tellg() - (streampos)offset;
					break;
				case POS2:
					graffa.filePos2 = file.tellg() - (streampos)offset;
					break;
				case POS3:
					graffa.filePos3 = file.tellg() - (streampos)offset;
					break;
			}

			blocchi->push_back(graffa);
		}

		bool checkExtraChars(string str, string keyword, size_t pos) {
			return (str.find_first_not_of(" \t") == pos && str.substr(pos + keyword.length()).find_first_not_of(" \t") == string::npos);
		}

		void addError(enum errorCode errore) {
			erroreFull codice;
			codice.row = row;
			codice.code = errore;
			codici->add(codice);	//ciao again ;)
		}

		bool varsContains(string str) {
			for (size_t i = 0; i < vars->size(); i++) {
				if ((vars->at(i)).nome == str) {
					return true;
				}
			}
			return false;
		}

		void checkInConstruct() {
			for (bool inizio = false; file; row++, getline(file, read)) {
				if (read.find_first_not_of(" \t") != string::npos) {
					if (inizio ? read.find(FINE) != string::npos : false) {
						break;
					}
					if (read.find(INIZIO) != string::npos) {
						inizio = true;
					}
					if (inizio) {
						selectStructure();
					} else {
						selectStructure();
						row++;
						break;
					}
				}
			}
		}

		void checkIF() {
			graffe.push(_SE);

			addGraffaPosition(POS1, OFFSET);
			index++;

			if (read.find_first_not_of(" \t") < pos) {	//ciao :) <3
				addError(GEN_EXTRA_CHAR);
			}
			read = read.substr(pos + string(SE).length());
			pos = read.find('(');
			pos2 = read.rfind(')');
			if (pos != string::npos && pos2 != string::npos) {
				if (pos > pos2) {
					addError(EXP_MISMATCH_PARENTESI);
				}
				if (pos + 1 == pos2) {
					addError(EXP_MISSING_CONDITION);
				}
				if ((read.substr(0, pos)).find_first_not_of(" \t") != string::npos) {
					addError(GEN_EXTRA_CHAR);
				}
				if (read.substr(pos2 + 1).find_first_not_of(" \t") != string::npos) {
					addError(GEN_EXTRA_CHAR);
				}
			} else {
				if (pos == string::npos) {
					addError(EXP_MISSING_APERTA);
				}
				if (pos2 == string::npos) {
					addError(EXP_MISSING_CHIUSA);
				}
			}

			checkIFBlock(index - 1);
		}

		void checkIFBlock(int structIndex) {
			//CHECK ALLORA
			for (getline(file, read); file; row++, getline(file, read)) {
				if (read.find_first_not_of(" \t") != string::npos) {
					if ((pos = read.find(ALLORA)) != string::npos) {
						if (!checkExtraChars(read, ALLORA, pos)) {
							addError(GEN_EXTRA_CHAR);
						}
					} else {
						addError(SYN_INCORRECT_STRUCTURE);
					}
					break;
				}
			}

			//CHECK INIZIO1
			readNext();
			checkInConstruct();

			//CHECK DIVISIONE (ALTRIMENTI - FINE_SE)
			for (readNext(); file; row++, getline(file, read)) {
				if (read.find_first_not_of(" \t") != string::npos) {
					if ((pos = read.find(ALTRIMENTI)) != string::npos) {
						if (!checkExtraChars(read, ALTRIMENTI, pos)) {
							addError(GEN_EXTRA_CHAR);
						}
						(blocchi->at(structIndex)).filePos2 = file.tellg();
						row++;
						break;
					} else {
						checkFineSe(structIndex);
					}
					break;
				}
			}

			if ((blocchi->at(structIndex)).filePos2 != -1) {
				//CHECK INIZIO2
				readNext();
				checkInConstruct();

				//CHECK FINE_SE
				for (readNext(); file; row++, getline(file, read)) {
					if (read.find_first_not_of(" \t") != string::npos) {
						checkFineSe(structIndex);
						break;
					}
				}
			}

			graffe.pop();
		}

		void checkFineSe(int structIndex) {
			if ((pos = read.find(FINE_SE)) != string::npos) {
				if (!checkExtraChars(read, FINE_SE, pos)) {
					addError(GEN_EXTRA_CHAR);
				}
				(blocchi->at(structIndex)).filePos3 = file.tellg();
			} else {
				addError(SYN_INCORRECT_STRUCTURE);
			}
		}

		void readNext() {
			if (file) {
				row++;
				getline(file, read);
			}
		}

		void checkFINTANTOCHE() {
			size_t pos3;
			graffe.push(_FINTANTOCHE);

			addGraffaPosition(POS1, OFFSET);
			index++;

			if (read.find_first_not_of(" \t") < pos) {	//ciao :) <3
				addError(GEN_EXTRA_CHAR);
			}
			read = read.substr(pos + string(FINTANTOCHE).length());
			pos = read.find('(');
			pos2 = read.rfind(')');
			pos3 = read.find(ESEGUI);
			if (pos != string::npos && pos2 != string::npos && pos3 != string::npos) {
				if (pos > pos2) {
					addError(EXP_MISMATCH_PARENTESI);
				}
				if (pos + 1 == pos2) {
					addError(EXP_MISSING_CONDITION);
				}
				if ((read.substr(0, pos)).find_first_not_of(" \t") != string::npos) {
					addError(GEN_EXTRA_CHAR);
				}
				if (read.substr(pos2 + 1, pos3 - (pos2 + 1)).find_first_not_of(" \t") != string::npos) {
					addError(GEN_EXTRA_CHAR);
				}
			} else {
				if (pos == string::npos) {
					addError(EXP_MISSING_APERTA);
				}
				if (pos2 == string::npos) {
					addError(EXP_MISSING_CHIUSA);
				}
			}
			if (pos3 != string::npos) {
				if (read.find(FINTANTOCHE) == string::npos && (pos = read.find(ESEGUI)) != string::npos) {
					if (read.substr(read.find(ESEGUI) + string(ESEGUI).length()).find_first_not_of(" \t") != string::npos) {
						addError(GEN_EXTRA_CHAR);
					}
				} else {
					addError(SYN_INCORRECT_STRUCTURE);
				}
			} else {
				addError(SYN_INCORRECT_STRUCTURE);
			}

			checkFINTANTOCHEBlock(index - 1);
		}

		void checkFINTANTOCHEBlock(int structIndex) {
			readNext();
			checkInConstruct();
			(blocchi->at(structIndex)).filePos3 = file.tellg();
			graffe.pop();
		}

		void checkFINOACHE() {
			graffe.push(_FINO_A_CHE);

			addGraffaPosition(POS1, 0);
			index++;

			if (!checkExtraChars(read, RIPETI, pos)) {
				addError(GEN_EXTRA_CHAR);
			}

			checkFINOACHEBlock(index - 1);
		}

		void checkFINOACHEBlock(int structIndex) {
			for (readNext(); file; row++, getline(file, read)) {
				selectStructure();

				if ((pos = read.find(FINO_A_CHE)) != string::npos) {
					(blocchi->at(structIndex)).filePos3 = file.tellg() - (streampos)OFFSET;

					if (read.find_first_not_of(" \t") < pos) {
						addError(GEN_EXTRA_CHAR);
					}
					read = read.substr(pos + string(FINO_A_CHE).length());
					pos = read.find('(');
					pos2 = read.rfind(')');
					if (pos != string::npos && pos2 != string::npos) {
						if (pos > pos2) {
							addError(EXP_MISMATCH_PARENTESI);
						}
						if (pos + 1 == pos2) {
							addError(EXP_MISSING_CONDITION);
						}
						if ((read.substr(0, pos)).find_first_not_of(" \t") != string::npos) {
							addError(GEN_EXTRA_CHAR);
						}
						if (read.substr(pos2 + 1).find_first_not_of(" \t") != string::npos) {
							addError(GEN_EXTRA_CHAR);
						}
					} else {
						if (pos == string::npos) {
							addError(EXP_MISSING_APERTA);
						}
						if (pos2 == string::npos) {
							addError(EXP_MISSING_CHIUSA);
						}
					}
					row++;
					break;
				}
			}

			graffe.pop();
		}

		void checkASSEGNAZIONE() {
			pos2 = read.find_first_not_of(" \t");
			if (pos2 < pos) {
				if ((read[pos2] >= 'a' && read[pos2] <= 'z') || (read[pos2] >= 'A' && read[pos2] <= 'Z')) {
					read = read.substr(pos2);
					read = read.substr(read.find_first_of(" \t<"));
					if ((pos = read.find(ASSEGNAZIONE)) != 0) {
						pos2 = read.find_first_not_of(" \t");
						if (pos2 < pos) {
							addError(GEN_EXTRA_CHAR);
						}
					}
					pos2 = read.substr(pos + string(ASSEGNAZIONE).length()).find_first_not_of(" \t");
					if (pos2 == string::npos) {
						addError(SYN_EXP_WRONG);
					}
				} else {
					addError(SYN_EXP_WRONG);
				}
			} else {
				addError(SYN_EXP_WRONG);
			}
		}

		void checkLEGGI() {
			if (read.find_first_not_of(" \t") < pos) {
				addError(GEN_EXTRA_CHAR);
			}
			read = read.substr(pos + 5);
			if ((pos = read.rfind(",")) != string::npos) {
				read = read.substr(pos + 1);
			}
			if ((pos = read.find_first_not_of(" \t")) == string::npos) {
				addError(SYM_SYNTAX);
			} else {
				read = read.substr(pos);
				if ((pos = read.find_first_of(" \t")) != string::npos) {
					if (read.substr(pos).find_first_not_of(" \t") != string::npos) {
						addError(GEN_EXTRA_CHAR);
					}
				}
			}
		}

		void checkSCRIVI() {
			if (read.find_first_not_of(" \t") < pos) {
				addError(GEN_EXTRA_CHAR);
			}
			read = read.substr(pos + 6);

			vector<string> tokens;
			stack<char> virgole;
			string temp;
			size_t pos1, pos2;
			int counter;

			for (size_t i = 0; read[i]; i++) {
				if ((read[i] == '\"' && i == 0) || (read[i] == '\"' && read[i-1] != '\\')) {
					if (virgole.empty()) {
						virgole.push(',');
					} else {
						virgole.pop();
					}
				}
				if (read[i] == ',' && !virgole.empty()) {
					read = read.insert(i, "\\");
					i++;
				}
			}

			if (!virgole.empty() || read.back() == ',') {
				addError(SYM_SYNTAX);
			}

			std::stringstream stringa(read);

			while (getline(stringa, temp, ',')) {
				tokens.push_back(temp);
			}

			for (size_t i = 0; i < tokens.size(); i++) {
				if (tokens[i].find_first_not_of(" \t") == string::npos) {
					addError(SYM_SYNTAX);
				} else if (tokens[i].find("\"") == string::npos && ((pos1 = tokens[i].rfind("\\")) == string::npos ? true : pos1 != tokens[i].length() - 1) && tokens[i].back() != '\\') {
					tokens[i] = tokens[i].substr(tokens[i].find_first_not_of(" \t"));
					pos1 = tokens[i].find_first_of(" \t");
					if (pos1 != string::npos) {
						tokens[i] = tokens[i].substr(0, pos1);
					}
					if (!isVarNameCorrect(tokens[i])) {
						addError(SYM_SYNTAX);
					}
				} else {
					pos1 = tokens[i].find_first_not_of(" \t");
					tokens[i] = tokens[i].substr(pos1);
					while ((pos1 = tokens[i].find("\\\"")) != string::npos) {
						tokens[i].replace(pos1, 2, "x");
					}

					counter = 0;
					for (size_t j = 0; j < tokens[i].length(); j++) {
						if (tokens[i].at(j) == '\"') {
							counter++;
						}
					}

					if (counter > 2) {
						addError(SYM_SYNTAX);
					} else {
						pos1 = tokens[i].find("\"");
						if (pos1 != string::npos) {
							if (pos1 == 0) {
								pos2 = tokens[i].rfind("\"");
								if (pos2 == pos1) {
									if (tokens[i].back() != '\\') {
										addError(SYM_SYNTAX);
									}
								} else {
									if (tokens[i].substr(pos2 + 1).find_first_not_of(" \t") != string::npos) {
										addError(SYM_SYNTAX);
									}
								}
							} else {
								if (tokens[i].substr(pos1 + 1).find_first_not_of(" \t") != string::npos) {
									addError(SYM_SYNTAX);
								}
							}
						} else {
							if (tokens[i].back() != '\\') {
								addError(SYM_SYNTAX);
							}
						}
					}
				}
			}
		}

};

#endif
