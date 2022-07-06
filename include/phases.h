#ifndef __phases_h__
#define __phases_h__

#include <iostream>
#include <cstdio>
#include <cstring>
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
#include "calculator.h"

using std::string;
using std::ifstream;
using std::ofstream;
using std::fstream;
using std::vector;
using std::stack;

class Phases {
	public:
		static bool correctFile(string fileName) {
			
			ifstream in(fileName, std::ios::binary);
		    if (!in.is_open()) {
		    	return false;
			}
		    
		    vector<unsigned char> content(std::istreambuf_iterator<char>(in), {});
		    
		    in.close();
		    
		    bool isWinFile = false;
		    size_t size = content.size();
		    
		    for (size_t i = 0; i < size && !isWinFile; i++) {
		    	if (content[i] == '\r') {
		    		isWinFile = true;
				}
		    }
		    
		    if (isWinFile) {
		    	ofstream out(fileName, std::ios::binary);
			    if (!out.is_open()) {
			        return false;
			    }
			    
				for (size_t i = 0; i < size; i++) {
			    	if (content[i] != '\r') {
			    		out<<content[i];
					}
			    }
			    
			    out.close();
			}
		    
		    return true;
		    
		}

	protected:
		vector<variabile> *vars;				//Lista di variabili
		Errori *codici;							//Codici di errore [errori.h]
		Calcolatore *calcolatrice;				//Blocco di calcolo
		string fileName;						//nome del file da leggere
		ifstream file;							//file da leggere
		string read;							//Stringa letta
		int row;								//Riga letta
		size_t pos;								//Variabile temporanea
		size_t pos2;							//Appoggio alla variabile temporanea
		
		void trim(string &str) {
			if (!str.empty()) {
				str = str.substr(str.find_first_not_of(" \t") != string::npos ? str.find_first_not_of(" \t") : 0);
				if (str.back() == ' ' || str.back() == '\t') {
					str = str.substr(0, (str.find_last_not_of(" \t") != string::npos ? str.find_last_not_of(" \t") + 1 : str.length() + 1));
				}
			}
		}

		bool openFile() {
			file.open(fileName);
			return file.is_open();
		}

		void repeatFile() {
			file.clear();
			file.seekg(0);
			row = 0;
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

		double varValue(int index) {
			return (vars->at(index)).valore;
		}

		int varIndex(string str) {
			for (size_t i = 0; i < vars->size(); i++) {
				if ((vars->at(i)).nome == str) {
					return i;
				}
			}
			return -1;
		}

		void addVariable(string str) {
			trim(str);
			if (isVarNameCorrect(str)) {
				variabile var;
				var.nome = str;
				var.valore = 0;
				vars->push_back(var);	
			} else {
				addError(VAR_NAME_WRONG);
			}
		}

		bool isVarNameCorrect(string str) {
			if (isAccettable(str) && !isOperator(str) && !isKeyWord(str)) {
				if (str[0] > '9') {
					return true;
				} else {
					return false;
				}
			} else {
				return false;
			}
		}
		
		bool isKeyWord(string str) {
			return str == INIZIO || str == FINE || str == SE || str == ALLORA || str == ALTRIMENTI || str == FINE_SE || str == RIPETI || str == FINO_A_CHE || str == FINTANTOCHE || str == ESEGUI || str == LEGGI || str == SCRIVI;
		}
		
		bool isNumber(string str) {
			size_t pos;
			return str.find_first_not_of("-0123456789.") == string::npos && str.front() != '.' && str.back() != '.' && ((pos = str.find(".")) != string::npos ? str.rfind(".") == pos : true) && ((pos = str.find("-")) != string::npos ? str.rfind("-") == pos && pos == 0 : true);
		}
		
		bool isLogicOperator(string str) {
			return str == EQ || str == NEQ || str == MAG || str == MAGEQ || str == MIN || str == MINEQ || str == AND || str == OR || str == NOT || str == XOR;
		}

		bool isMathOperator(string str) {
			return str == PIU || str == MENO || str == PER || str == DIVISO || str == MOD;
		}

		bool isOperator(string str) {
			return isLogicOperator(str) || isMathOperator(str);
		}
		
		bool isAccettable(string str) {
			return str.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_") == string::npos;
		}

		string nextWordScrivi(string& str) {
			
			string temp;

			str = str.substr(str.find_first_not_of(" \t"));

			if ((pos = str.find("\"")) != string::npos && str.find_first_not_of(" \t") == pos) {
				str = str.substr(str.find("\"", pos));
				if (str.substr(0, (pos = str.find(","))).find_first_of(" \t") == string::npos) {
					str.substr(pos + 1);
				}
			}

			if ((pos = str.find(",")) != string::npos) {
				temp = str.substr(0, pos);
				str = str.substr(pos + 1);
				return temp;
			}

			temp = str;
			str.clear();

			return temp;

		}

};

#endif
