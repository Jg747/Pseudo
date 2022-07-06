#ifndef calculator
#define calculator

#include <iostream>
#include <stack>
#include <cstdlib>
#include <limits>
#include <cmath>

#include <structures.h>
#include <errori.h>

using std::cout;
using std::string;
using std::stack;
using std::to_string;

class Calcolatore {
	private:
		string exp;								//Expression
		string expBackup;						//Copy of the expression
		vector<variabile> *vars;				//Variables
		Errori *codici;
		erroreFull codice;						//Error code

		// Check if varName (str) exists in the array
		bool varsContains(string str);

		// Add error to error list
		void addError(enum errorCode errore);

		// Exp manager
		void resetExpression();
		string getExpression();

		// Bool checks
		bool isAccettable(string str);
		bool isKeyWord(string str);
		bool isLogicOperator(string str);
		bool isMathOperator(string str);
		bool isOperator(string str);
		bool skipChar(char car);
		
		string nextWord(string& str);

		// Var array
		double varValueC(int index);
		int varIndexC(string str);
	
	public:
		// Init calculator
		Calcolatore(vector<variabile> *vars, Errori *codici);
		void setExpression(string str, int row);

		// Exp
		bool evaluate();
		double calculate();

		// Convert exp
		static string modifyExpressionFromPseudo(string str);
		static string modifyExpressionToPseudo(string str);

		// Bool checks
		static bool isNumber(string str);
		static bool isValid(string str);

		// Add not behind exp
		static string invertCondition(string str);

};

#endif
