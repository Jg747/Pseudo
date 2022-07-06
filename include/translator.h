#ifndef __translator_h__
#define __translator_h__

#include "cpptopseudo.h"
#include "pseudotocpp.h"

using std::string;
using std::ifstream;
using std::ofstream;

class Translator {
	public:
		Translator(int mode, string fileName) {
			this->mode = mode;
			this->fileName = fileName;
			this->outName = fileName.substr(0, fileName.rfind(".")) + (fileName.find(".cpp") != string::npos ? ".txt" : ".cpp");
		}

		bool traduci() {

			bool isClean = false;
			CppToPseudo *t1;
			PseudoToCpp *t2;

			if (mode == 0) {
				//C++ -> Pseudo
				t1 = new CppToPseudo(fileName, outName);
				isClean = t1->cppToPseudo();
				delete t1;
			} else if (mode == 1) {
				//Pseudo -> C++
				t2 = new PseudoToCpp(fileName, outName);
				isClean = t2->pseudoToCpp();
				delete t2;
			}

			return isClean;

		}
	private:
		int mode;
		string fileName;
		string outName;
};

#endif