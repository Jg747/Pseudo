#ifndef __cpp_pseudo__
#define __cpp_pseudo__

#include <iostream>
#include <fstream>
#include <string>
#include <stack>

#include "lang.h"
#include "structures.h"
#include "calculator.h"

using std::string;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::cin;
using std::stack;

class CppToPseudo {
	public:
		CppToPseudo(string fileName, string outName) {
			this->fileName = fileName;
			this->outName = outName;
		}

		bool cppToPseudo() {

			size_t pos;

			file.open(fileName);
			out.open(outName);

			if (!file.is_open() || !out.is_open()) {
				return false;
			}
			
			mainEncountered = false;

			writeLine((string(INIZIO) + "\n"));
			tabs.push(_NO);

			for (getline(file, read); file; getline(file, read)) {
				if (!skipRow()) {
					//Gestire operatori di assegnazione [=, +=, -=, *=, /=, ++, --]
					//Gestire strutture [if, if-else, while - for, do while]
					//cin
					//cout
					if (!read.empty() && read.find_first_not_of(" \t") != string::npos) {
						read = read.substr(read.find_first_not_of(" \t"));
					}
					if ((read.empty() || read.find_first_not_of(" \t") == string::npos) && mainEncountered) {
						writeLine("\n");
					} else {
						if ((pos = read.find("//")) != string::npos) {
							if (pos > read.find(";")) {
								read = read.substr(0, pos);
							}
						}
						selectStructure();
					}
				}
			}

			file.close();
			out.close();

			return true;

		}
		
	private:
		string fileName;
		string outName;
		stack<enum stato> tabs;
		ifstream file;
		ofstream out;
		string read;
		size_t pos1;
		stack<vector<string>> operazioni_for;
		bool mainEncountered;
		int ifCount;
		stack<int> ifVari;

		void trim(string &str) {
			if (!str.empty()) {
				str = str.substr(str.find_first_not_of(" \t") != string::npos ? str.find_first_not_of(" \t") : 0);
				if (str.back() == ' ' || str.back() == '\t') {
					str = str.substr(0, (str.find_last_not_of(" \t") != string::npos ? str.find_last_not_of(" \t") + 1 : str.length() + 1));
				}
			}
		}

		bool skipRow() {
			if (read.find("using") != string::npos) {
				return true;
			} else if (read.find("namespace") != string::npos) {
				return true;
			} else if (read.find("include") != string::npos) {
				return true;
			} else if (read.find("return") != string::npos) {
				return true;
			} else if ((pos1 = read.find("{")) != string::npos && read.find_first_not_of(" \t") == pos1) {
				return true;
			} else if (read.find("main()") != string::npos || read.find("main(void)") != string::npos || read.find("main(int argc, char* argv[])") != string::npos) {
				mainEncountered = true;
				writeLine("\n");
				return true;
			}
			return false;
		}

		int isAssegnazione(string str) {
			size_t pos;
			if (str.find("for") == string::npos) {
				if (str.find("++") != string::npos || str.find("--") != string::npos) {
					return 3;
				}
				if (str.find("+=") != string::npos || str.find("-=") != string::npos || str.find("*=") != string::npos || str.find("/=") != string::npos) {
					return 2;
				}
				pos = str.find("if");
				if (str.find("=") != string::npos && (pos == string::npos ? true : !((read[pos+2] == ' ' || read[pos+2] == '\t' || read[pos+2] == '(') && (pos > 0 ? (read[pos-1] == ' ' || read[pos-1] == '\t') : true))) && str.find("while") == string::npos && str.find("for") == string::npos) {
					return 1;
				}
			}
			return 0;
		}

		string writeTabs() {
			string str = "";
			for (int i = 0; i < tabs.size(); i++) {
				str += "\t";
			}
			return str;
		}

		bool eseguiAssegnazione(string &str) {
			string temp, temp2;
			int assegnazione;
			char segno;
			vector<string> v;
			string scrivi = "";
			bool first = false;

			if ((assegnazione = isAssegnazione(str))) {

				trim(str);

				str = removeType(str);

				while (str.find(",") != string::npos) {
					temp = str.substr(0, str.find(","));
					trim(temp);
					v.push_back(temp);
					str = str.substr(str.find(",") + 1);
				}
				temp = str.substr(0, str.find(";"));
				trim(temp);
				v.push_back(temp);

				for (string appoggio : v) {
					str = appoggio;
					if (str.find("=") != string::npos) {
						temp = str.substr(0, str.find("="));
						str = str.substr(str.find("=") + 1);
					} else {
						temp = str;
					}
					trim(str);

					if (assegnazione == 1) {
						
						if (str.find("=") != string::npos) {
							trim(temp);
							temp2 = str.substr(str.rfind("=") + 1);
							temp2 = temp2.substr(0, temp2.find(";"));
							trim(temp2);
							temp2 = Calcolatore::modifyExpressionToPseudo(temp2);
							while (str.find("=") != string::npos) {
								temp += " <- ";
								temp += temp2;
								trim(temp);
								scrivi += writeTabs() + temp + "\n";
								temp = str.substr(0, str.find("="));
								str = str.substr(str.find("=") + 1);
							}
						}

						trim(temp);

						temp += " <- ";

					} else if (assegnazione >= 2) {

						segno = temp.back();
						temp.pop_back();

						if (assegnazione == 3) {
							temp2 = temp.substr(temp.find_last_not_of(" \t"), 1);
							if (temp2 == "+") {
								segno = 1;
							} else {
								segno = 2;
							}
							temp = temp.substr(0, temp.find_last_not_of(" \t"));
							if (temp.back() == '+' || temp.back() == '-') {
								temp.pop_back();
							}
						}

						trim(temp);

						temp = temp.substr(0, temp.find(segno));

						switch (segno) {
							case '+':
								temp += " <- " + temp + " + ";
								break;
							case '-':
								temp += " <- " + temp + " - ";
								break;
							case '*':
								temp += " <- " + temp + " * ";
								break;
							case '/':
								temp += " <- " + temp + " / ";
								break;
							case 1:
								temp += " <- " + temp + " + 1";
								break;
							case 2:
								temp += " <- " + temp + " - 1";
								break;
						}

					}

					if (assegnazione != 3) {
						str = Calcolatore::modifyExpressionToPseudo(str);
						trim(str);

						temp += str;
						temp = temp.substr(0, temp.find(";"));
						trim(temp);

						string check = temp;
						check.replace(check.find(" " + string(ASSEGNAZIONE) + " "), 2+string(ASSEGNAZIONE).length(), "\x01");
						check = check.substr(check.find("\x01") + 1);
						if (temp.substr(0, temp.find(" " + string(ASSEGNAZIONE) + " ")) == check) {
							temp = temp.substr(0, temp.find(" " + string(ASSEGNAZIONE) + " ")) + " " + string(ASSEGNAZIONE) + " 0";
						}
					}

					if (!first) {
						scrivi += temp + "\n";
						first = true;
					} else {
						scrivi += writeTabs() + temp + "\n";
					}
				}

				str = scrivi;

				return true;
			} else {
				return false;
			}
		}

		string removeType(string str) {
			size_t pos;
			if ((pos = str.find("int ")) != string::npos) {
				if (str.find_first_not_of(" \t") == pos) {
					str = str.substr(pos + 4);
				}
			} else if ((pos = str.find("float ")) != string::npos) {
				if (str.find_first_not_of(" \t") == pos) {
					str = str.substr(pos + 6);
				}
			} else if ((pos = str.find("double ")) != string::npos) {
				if (str.find_first_not_of(" \t") == pos) {
					str = str.substr(pos + 7);
				}
			} else if ((pos = str.find("bool ")) != string::npos) {
				if (str.find_first_not_of(" \t") == pos) {
					str = str.substr(pos + 5);
				}
			} else if ((pos = str.find("char ")) != string::npos) {
				if (str.find_first_not_of(" \t") == pos) {
					str = str.substr(pos + 5);
				}
			}
			return str;
		}

		void writeLine(string str) {
			str = writeTabs() + str;
			out << str;
		}

		void writeForOperations(vector<string> stringhe) {
			for (size_t i = 0; i < stringhe.size(); i++) {
				writeLine(stringhe[i]);
			}
		}

		void scriviDefine() {
			string temp;

			read = read.substr(pos1 + 7);
			read = read.substr(read.find_first_not_of(" \t"));
			temp = read.substr(0, read.find_first_of(" \t"));
			read = read.substr(read.find_first_of(" \t"));
			read = read.substr(read.find_first_not_of(" \t"));
			if (read.find("\"") == string::npos) {
				if ((pos1 = read.find_first_of("LFlf")) != string::npos) {
					read = read.substr(0, pos1);
				} else {
					read = read.substr(0, read.find_first_of(" \t"));
				}
				temp = "\t" + temp + " <- " + read;
				out<<temp<<"\n";
			}
		}

		void scriviCin() {
			string temp, varTemp;

			temp = string(LEGGI);
			read = read.substr(read.find(">>") + 2);
			trim(read);
			while ((pos1 = read.find(">>")) != string::npos) {
				varTemp = read.substr(0, pos1);
				trim(varTemp);
				temp += varTemp + ", ";
				read = read.substr(pos1 + 2);
			}
			trim(read);
			read.pop_back();
			trim(read);
			temp += read + "\n";
			
			writeLine(temp);
		}

		void scriviCout() {
			string temp, varTemp;
			bool virgolette;
			size_t pos2;

			const string symbols = "\x01\x02\x03"; //"\x01\x02\x03"; //"<<" [\x01], " " [\x02], "\t" [\x03]

			temp = string(SCRIVI);
			read = read.substr(read.find("<<") + 2);
			virgolette = false;
			for (size_t i = 0; read[i]; i++) {
				if (read[i] == '\"' && read[i-1] != '\\') {
					if (virgolette) {
						virgolette = false;
					} else {
						virgolette = true;
					}
				}
				if (virgolette) {
					if (read[i] == ' ') {
						read[i] = symbols[1];
					} else if (read[i] == '\t') {
						read[i] = symbols[2];
					} else if (read.substr(i, 2) == "<<") {
						read.replace(i, 2, symbols.substr(0, 1));
					}
				}
			}

			trim(read); //"ci.ao"<<"\"|."; "ci.ao" <<"\"|."; "ci.ao"<< "\"|."; "ci.ao" << "\"|.";
			while (read.find("<<") != string::npos) {
				pos1 = read.find_first_of(" \t");
				pos2 = read.find("<<");
				if (pos1 < pos2 && pos2 != string::npos) {
					read.erase(pos1, pos2-pos1);
					pos1 = read.find_first_of(" \t");
					pos2 = read.find("<<");
				}
				if (pos2 != string::npos) {
					read.replace(pos2, 2, symbols.substr(0, 1));
					if (pos1 != string::npos) {
						if (pos1 < pos2) {
							pos2 = read.find_first_not_of(" \t");
							if (pos2 != string::npos) {
								read.erase(pos1+1, pos2-(pos1+1));
							}
							read.insert(pos1+1, symbols.substr(1, 1));
						} else {
							pos1 = read.find_first_of(" \t");
							read.erase(pos1, read.find_first_not_of(" \t", pos1)-pos1);
							read.insert(pos1, symbols.substr(1, 1));
						}
					} else {
						read.insert(pos2+1, symbols.substr(1, 1));
					}
				}
			}
			trim(read);

			virgolette = false;
			for (size_t i = 0; read[i]; i++) {
				if (read[i] == '\"' && read[i-1] != '\\') {
					if (virgolette) {
						virgolette = false;
					} else {
						virgolette = true;
					}
				}
				if (virgolette) {
					if (read[i] == symbols[0]) {
						read.replace(i, 1, "<<");
						i++;
					} else if (read[i] == symbols[1]) {
						read[i] = ' ';
					} else if (read[i] == symbols[2]) {
						read[i] = '\t';
					}
				} else if (!virgolette) {
					if (read[i] == symbols[0]) {
						read.replace(i, 1, ",");
					} else if (read[i] == symbols[1] || read[i] == symbols[2]) {
						read[i] = ' ';
					}
				}
			}

			temp += read;
			temp.pop_back();
			temp += "\n";
			
			writeLine(temp);

		}

		void scriviGraffa() {
			string temp;

			if (read.find("else") != string::npos) {
				scriviElse();
			} else if (read.find("while") != string::npos) {
				//WHILE (proveniente da un Do (do-while))
				scriviDoWhile(true);
			} else {
				//Chiusura classica
				tabs.pop();
				if (!tabs.empty()) {
					if (tabs.top() == _FINTANTOCHE) {
						tabs.push(_FINE);
						writeForOperations(operazioni_for.top());
						operazioni_for.pop();
						tabs.pop();
						writeLine((string(FINE) + "\n"));
						tabs.pop();
					} else {
						writeLine((string(FINE) + "\n"));
						tabs.pop();
						if (tabs.top() == _ALLORA) {
							popIfVari();
						}
					}
				} else {
					writeLine((string(FINE) + "\n"));
				}
			}
		}

		void popIfVari() {
			tabs.pop();
			writeLine((string(FINE_SE) + "\n"));
			for (ifCount = ifVari.top() - 1; ifCount >= 0; ifCount--) {
				if (tabs.top() == _FINE2) {
					tabs.pop();
				}
				writeLine((string(FINE) + "\n"));
				tabs.pop();
				tabs.pop();
				writeLine((string(FINE_SE) + "\n"));
			}
			ifVari.pop();
		}

		void scriviIf() {
			string temp;
			bool graphNotPresent = false;
			streampos tempPos;

			if (read.find("{") == string::npos) {
				graphNotPresent = true;
			}

			temp = string(SE) + " " + string(INIZIO_EXP);
			read = read.substr(read.find('(') + 1);
			read = read.substr(0, read.rfind(')'));

			read = Calcolatore::modifyExpressionToPseudo(read);
			temp += read + string(FINE_EXP) + "\n";

			writeLine(temp);
			tabs.push(_ALLORA);

			temp = string(ALLORA) + "\n";
			writeLine(temp);
			tabs.push(_INIZIO);

			temp = string(INIZIO) + "\n";
			writeLine(temp);
			tabs.push(_FINE);

			if (graphNotPresent) {
				getline(file, read);
				selectStructure();
				tempPos = file.tellg();
				getline(file, read);
				if (read.find("else") == string::npos) {
					file.seekg(tempPos);
					tabs.pop();
					writeLine((string(FINE) + "\n"));
					tabs.pop();
					if (tabs.top() == _ALLORA) {
						popIfVari();
					}
				} else {
					file.seekg(tempPos);
				}
			}
		}

		void scriviElse() {
			bool graphNotPresent = false;
			streampos tempPos;

			if (read.find("{") == string::npos) {
				graphNotPresent = true;
			}

			if (read.find("if") == string::npos) {
				//ELSE (proveniente da un if)
				tabs.pop();
				writeLine((string(FINE) + "\n"));
				if (!tabs.empty()) {
					tabs.pop();
					writeLine((string(ALTRIMENTI) + "\n"));
					tabs.push(_INIZIO2);
					writeLine((string(INIZIO) + "\n"));
					tabs.push(_FINE2);

					if (graphNotPresent) {
						getline(file, read);
						selectStructure();
						tabs.pop();
						writeLine((string(FINE) + "\n"));
						tabs.pop();
						if (tabs.top() == _ALLORA) {
							popIfVari();
						}
					}
				}
			} else {
				//ELSE IF
				ifCount = ifVari.top();
				ifVari.pop();
				ifVari.push(ifCount + 1);
				tabs.pop();
				writeLine((string(FINE) + "\n"));
				if (!tabs.empty()) {
					tabs.pop();
					writeLine((string(ALTRIMENTI) + "\n"));
					tabs.push(_INIZIO2);
					writeLine((string(INIZIO) + "\n"));
					tabs.push(_FINE2);

					scriviIf();
				}
			}
		}

		void scriviDoWhile(bool isWhile) {
			string temp;

			if (!isWhile) {
				writeLine((string(RIPETI) + "\n"));
				tabs.push(_RIPETI);
			} else {
				temp = string(FINO_A_CHE) + " " + string(INIZIO_EXP);
				read = read.substr(read.find('(') + 1);
				read = read.substr(0, read.rfind(')'));
				read = Calcolatore::invertCondition(read);
				read = Calcolatore::modifyExpressionToPseudo(read);
				temp += read + string(FINE_EXP) + "\n";

				tabs.pop();
				writeLine(temp);
			}
		}

		void scriviWhile() {
			string temp;
			bool graphNotPresent = false;

			if (read.find("{") == string::npos) {
				graphNotPresent = true;
			}

			temp = string(FINTANTOCHE) + " " + string(INIZIO_EXP);
			read = read.substr(read.find('(') + 1);
			read = read.substr(0, read.rfind(')'));

			read = Calcolatore::modifyExpressionToPseudo(read);
			temp += read + string(FINE_EXP) + " " + string(ESEGUI) + "\n";

			writeLine(temp);

			temp = string(INIZIO) + "\n";
			tabs.push(_INIZIO);
			writeLine(temp);

			tabs.push(_FINE);

			if (graphNotPresent) {
				getline(file, read);
				selectStructure();
				tabs.pop();
				writeLine((string(FINE) + "\n"));
				tabs.pop();
			}

		}

		void scriviFor() {
			string temp, temp2;
			int structType = 0;
			vector<string> operazioni;

			//Controllo se il for sia solo una riga, mi serve per dopo
			pos1 = read.rfind(')');
			if (read.rfind(';') > pos1) {
				structType = 2;
			} else if (read.find('{') == string::npos) {
				structType = 1;
			}

			//Piglia il contenuto del for
			read = read.substr(read.find('(') + 1);
			read = read.substr(0, read.find(')'));

			//Inizializzazioni pre-for
			temp2 = read.substr(0, read.find(';'));
			read = read.substr(read.find(';') + 1);
			while ((pos1 = temp2.find(',')) != string::npos) {
				temp = temp2.substr(0, pos1);
				temp2 = temp2.substr(pos1 + 1);
				temp = removeType(temp);
				eseguiAssegnazione(temp);
				writeLine(temp);
			}
			temp2 = removeType(temp2);
			eseguiAssegnazione(temp2);
			writeLine(temp2);

			//Scrittura del for
			temp = read.substr(0, read.find(';'));
			read = read.substr(read.find(';') + 1);

			temp = Calcolatore::modifyExpressionToPseudo(temp);
			temp = temp.substr(temp.find_first_not_of(" \t"));
			temp = string(FINTANTOCHE) + " (" + temp + ") " + string(ESEGUI) + "\n";
			writeLine(temp);

			tabs.push(_FINTANTOCHE);
			writeLine((string(INIZIO) + "\n"));

			tabs.push(_FINE);

			//Operazioni alla fine del for
			operazioni.clear();
			temp2 = read.substr(read.find_first_not_of(" \t"));
			while ((pos1 = temp2.find(',')) != string::npos) {
				temp = temp2.substr(0, pos1);
				temp2 = temp2.substr(pos1 + 1);
				temp = removeType(temp);
				eseguiAssegnazione(temp);
				operazioni.push_back(temp);
			}
			temp2 = removeType(temp2);
			eseguiAssegnazione(temp2);
			operazioni.push_back(temp2);
			operazioni_for.push(operazioni);

			if (structType == 1) {	//Singola riga, senza {}
				getline(file, read);
				selectStructure();
				writeForOperations(operazioni_for.top());
				operazioni_for.pop();
				tabs.pop();
				writeLine((string(FINE) + "\n"));
				tabs.pop();
			} else if (structType == 2) {	//Singola riga, 0 istruzioni, ; alla fine
				writeForOperations(operazioni_for.top());
				operazioni_for.pop();
				tabs.pop();
				writeLine((string(FINE) + "\n"));
				tabs.pop();
			}
		}

		void selectStructure() {
			string temp;
			if (eseguiAssegnazione(read)) {
				writeLine(read);
			} else if ((pos1 = read.find("define")) != string::npos) {
				scriviDefine();
			} else if ((pos1 = read.find("cin")) != string::npos) {
				scriviCin();
			} else if ((pos1 = read.find("cout")) != string::npos) {
				scriviCout();
			} else if ((pos1 = read.find('}')) != string::npos) {
				scriviGraffa();
			} else {
				pos1 = read.find("if");
				if (pos1 != string::npos && read.find("else") == string::npos && (read[pos1+2] == ' ' || read[pos1+2] == '\t' || read[pos1+2] == '(') && (pos1 > 0 ? (read[pos1-1] == ' ' || read[pos1-1] == '\t') : true)) {
					ifVari.push(0);
					scriviIf();
				} else if ((pos1 = read.find("else")) != string::npos) {
					scriviElse();
				} else if ((pos1 = read.find("do")) != string::npos && read.find("double") == string::npos) {
					scriviDoWhile(false);
				} else if ((pos1 = read.find("while")) != string::npos) {
					scriviWhile();
				} else if ((pos1 = read.find("for")) != string::npos) {
					scriviFor();
				} else if (read.find_first_not_of(" \t") != string::npos) {
					read = removeType(read);
					trim(read);
					read.pop_back();
					while ((pos1 = read.find(",")) != string::npos) {
						temp = read.substr(0, pos1);
						trim(temp);
						writeLine(temp + " " + string(ASSEGNAZIONE) + " 0\n");
						read = read.substr(pos1 + 1);
					}
					trim(read);
					writeLine(read + " " + string(ASSEGNAZIONE) + " 0\n");
				}
			}
		}

};

#endif