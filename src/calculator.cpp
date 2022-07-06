#include <calculator.h>

#include <iostream>
#include <stack>
#include <cstdlib>
#include <limits>
#include <cmath>

#include <main.h>
#include <structures.h>
#include <errori.h>

using std::cout;
using std::string;
using std::stack;
using std::to_string;

		bool Calcolatore::varsContains(string str) {
			for (size_t i = 0; i < vars->size(); i++) {
				if ((vars->at(i)).nome == str) {
					return true;
				}
			}
			return false;
		}
		
		void Calcolatore::addError(enum errorCode errore) {
			codice.code = errore;
			if (codici != NULL) {
				codici->add(codice);
			}
		}

		void Calcolatore::resetExpression() {
			exp = expBackup;
		}
		
		string Calcolatore::getExpression() {
			return expBackup;
		}

		bool Calcolatore::isAccettable(string str) {
			return str.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_") == string::npos;
		}

		bool Calcolatore::isKeyWord(string str) {
			return str == INIZIO || 
                   str == FINE || 
                   str == SE || 
                   str == ALLORA || 
                   str == ALTRIMENTI || 
                   str == FINE_SE || 
                   str == RIPETI || 
                   str == FINO_A_CHE || 
                   str == FINTANTOCHE || 
                   str == ESEGUI || 
                   str == LEGGI || 
                   str == SCRIVI ||
                   str == BEGIN ||
                   str == END ||
                   str == IF ||
                   str == THEN ||
                   str == ELSE ||
                   str == DO ||
                   str == WHILE ||
                   str == VARIABLE ||
                   str == FUNCTION ||
                   str == PROCEDURE ||
                   str == READ ||
                   str == WRITE ||
                   str == RETURN;
		}
		
		bool Calcolatore::isLogicOperator(string str) {
			return str == EQ || str == NEQ || str == MAG || str == MAGEQ || str == MIN || str == MINEQ || str == AND || str == OR || str == NOT || str == XOR;
		}

		bool Calcolatore::isMathOperator(string str) {
			return str == PLUS || str == MINUS || str == MULTIPL || str == DIVISION || str == MOD;
		}

		bool Calcolatore::isOperator(string str) {
			return isLogicOperator(str) || isMathOperator(str);
		}
		
		bool Calcolatore::skipChar(char car) {
			return car == ' ' || car == '\t' || car == '+' || car == '-' || car == '*' || car == '/' || car == '(' || car == ')';
		}
		
		string Calcolatore::nextWord(string& str) {
			
			string temp, word = "";
			size_t tempPos;

			tempPos = str.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_");
			if (tempPos == 0) {
				str = str.substr(1);
			}

			if (tempPos == string::npos) {
				temp = str;
				str.clear();
				if (!isOperator(temp) && !isNumber(temp))
					return temp;
				else
					return ".";
			}

			while (!str.empty()) {
				tempPos = str.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_");
				if (tempPos != string::npos) {
					temp = str.substr(0, tempPos);
					if (isAccettable(temp)) {
						if (!isOperator(temp) && !isNumber(temp)) {
							str = str.substr(temp.length());
							return temp;
						} else {
							str = str.substr(temp.length());
							break;
						}
					} else {
						if (temp.length() == 1 && str.length() > 1)
							str = str.substr(1);
						else
							str = str.substr(temp.length());
					}
				} else {
					break;
				}
			}

			return ".";

		}

		double Calcolatore::varValueC(int index) {
			return (vars->at(index)).valore;
		}

		int Calcolatore::varIndexC(string str) {
			for (size_t i = 0; i < vars->size(); i++) {
				if ((vars->at(i)).nome == str) {
					return i;
				}
			}
			return -1;
		}
	
		Calcolatore::Calcolatore(vector<variabile> *vars, Errori *codici) {
			this->vars = vars;
			this->codici = codici;
		}

		void Calcolatore::setExpression(string str, int row) {
			exp = str;
			expBackup = str;
			codice.row = row;
		}

		bool Calcolatore::evaluate() {

			resetExpression();

			size_t tempPos = exp.find_first_not_of(" \t");
			string editedExp;
			double result;

			if (tempPos != string::npos) {
				exp = exp.substr(tempPos);
			}

			editedExp = exp;

			editedExp = modifyExpressionFromPseudo(editedExp);

			for (string temp = nextWord(exp); ; temp = nextWord(exp)) {
				if (temp != ".") {
					if (!varsContains(temp)) {
						addError(VAR_NOT_EXIST);
					}
					editedExp.replace(editedExp.find(temp), temp.length(), "1");
				}
				if (exp.empty()) {
					break;
				}
			}
			
			try {
				result = (cparse::calculator::calculate(editedExp.c_str())).asDouble();
				if (result == std::numeric_limits<double>::infinity()) {
					addError(EXP_ZERO_DIVISION);
					return false;
				}
			} catch (const std::exception &e) {
				return false;
			}

			return true;

		}

		double Calcolatore::calculate() {
			
			size_t pos;
			variabile var;
			string editedExp;
			double result;

			resetExpression();

			if ((pos = exp.find_first_not_of(" \t")) != string::npos) {
				exp = exp.substr(pos);
			}

			editedExp = exp;

			editedExp = modifyExpressionFromPseudo(editedExp);

			for (string temp = nextWord(exp); ; temp = nextWord(exp)) {
				if (temp != ".") {
					editedExp.replace(editedExp.find(temp), temp.length(), to_string(varValueC(varIndexC(temp))));
				}
				if (exp.empty()) {
					break;
				}
			}

			try {
				result = (cparse::calculator::calculate(editedExp.c_str())).asDouble();
			} catch (const std::exception &e) {
				result = std::numeric_limits<double>::infinity();
				addError(EXP_UNKNOWN_EXCEPTION);
			}

			return result;

		}

		string Calcolatore::modifyExpressionFromPseudo(string str) {

			size_t pos;

			//Rimpiazza tutti gli "AND" -> "&&"
			while ((pos = str.find(" " + string(AND) + " ")) != string::npos) {
				str.replace(pos, string(AND).length() + 2, " && ");
			}

			//Rimpiazza tutti gli "OR" -> "||"
			while ((pos = str.find(" " + string(OR) + " ")) != string::npos) {
				str.replace(pos, string(OR).length() + 2, " || ");
			}

			//Rimpiazza tutti gli "XOR" -> "^"
			while ((pos = str.find(" " + string(XOR) + " ")) != string::npos) {
				str.replace(pos, string(XOR).length() + 2, " ^ ");
			}

			//Rimpiazza tutti i "MOD" -> "%"
			while ((pos = str.find(" " + string(MOD) + " ")) != string::npos) {
				str.replace(pos, string(MOD).length() + 2, " % ");
			}

			//Rimpiazza tutti i "NOT" -> "!"
			while ((pos = str.find(NOT)) != string::npos) {
				str.replace(pos, string(NOT).length(), "!");
			}

			//Rimpiazza tutti i "=" -> "=="
			pos = 0;
			while ((pos = str.find(EQ, pos)) != string::npos) {
				if (str.substr(pos, 2) != "==" && str.find(MAGEQ, pos-1) + 1 != pos && str.find(MINEQ, pos-1) + 1 != pos) {
					if (str[pos-1] != ' ' && str[pos-1] != '\t') {
						str.insert(pos, " ");
						pos++;
					}
					str.replace(pos, string(EQ).length(), "==");
					pos += 2;
					if (str[pos] != ' ' && str[pos] != '\t') {
						str.insert(pos, " ");
						pos++;
					}
				} else {
					pos += 2;
				}
			}

			//Rimpiazza tutti i "<>" -> "!="
			while ((pos = str.find(NEQ)) != string::npos) {
				str.replace(pos, string(NEQ).length(), " != ");
			}

			return str;

		}

		string Calcolatore::modifyExpressionToPseudo(string str) {

			size_t pos;
			string temp;

			//Rimpiazza tutti gli "&&" -> "AND"
			temp = " " + string(AND) + " ";
			while ((pos = str.find("&&")) != string::npos) {
				str.replace(pos, 2, temp);
			}

			//Rimpiazza tutti gli "||" -> "OR"
			temp = " " + string(OR) + " ";
			while ((pos = str.find("||")) != string::npos) {
				str.replace(pos, 2, temp);
			}

			//Rimpiazza tutti gli "^" -> "XOR"
			temp = " " + string(XOR) + " ";
			while ((pos = str.find("^")) != string::npos) {
				str.replace(pos, 1, temp);
			}

			//Rimpiazza tutti i "%" -> "MOD"
			temp = " " + string(MOD) + " ";
			while ((pos = str.find("%")) != string::npos) {
				str.replace(pos, 1, temp);
			}

			//Rimpiazza tutti i "!" -> "NOT"
			temp = " " + string(NOT) + " ";
			while ((pos = str.find("!")) != string::npos && str[pos+1] != '=') {
				str.replace(pos, 1, temp);
			}

			//Rimpiazza tutti i "!=" -> "<>"
			temp = string(NEQ);
			while ((pos = str.find("!=")) != string::npos) {
				str.replace(pos, 2, temp);
			}

			//Rimpiazza tutti i "==" -> "="
			temp = string(EQ);
			while ((pos = str.find("==")) != string::npos) {
				str.replace(pos, 2, temp);
			}

			return str;

		}

		bool Calcolatore::isNumber(string str) {
			size_t pos;
			return str.find_first_not_of("0123456789.") == string::npos && str.front() != '.' && str.back() != '.' && ((pos = str.find(".")) != string::npos ? str.rfind(".") == pos : true);
		}

		bool Calcolatore::isValid(string str) {
			try {
				double result = (cparse::calculator::calculate(str.c_str())).asDouble();
				if (result != std::numeric_limits<double>::infinity()) {
					return true;
				} else {
					return false;
				}
			} catch (const std::exception &e) {
				return false;
			}
			return true;
		}

		string Calcolatore::invertCondition(string str) {
			return "!(" + str + ")";
		}
