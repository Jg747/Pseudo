#ifndef __structures_h__
#define __structures_h__

#include <string>
#include <fstream>

using std::streampos;
using std::string;

struct variabile {
	string nome;
	double valore;
};

bool operator==(variabile &v1, variabile &v2) {
	return v1.nome == v2.nome;
}

enum stato {
	_NO,					//0
	_SE,					//1
		_ALLORA,			//2
		_ALTRIMENTI,		//3
		_FINE_SE,			//4
		_DIVIDI,			//5
	_FINO_A_CHE,			//6
		_RIPETI,			//7
		_FINO,				//8
	_FINTANTOCHE,			//9
	_INIZIO,				//10
	_FINE,					//11
	_INIZIO2,				//12
	_FINE2,					//13
};

enum filePosition {
	POS1,
	POS2,
	POS3
};

enum blockType {
	IF,
	IF_ELSE,
	WHILE,
	DOWHILE
};

struct blocco {
	enum stato costrutto;
	size_t index;
};

class Blocco {
	public:
		streampos filePos1;
		streampos filePos2;
		streampos filePos3;
		int row;

		Blocco() {
			filePos1 = -1;
			filePos2 = -1;
			filePos3 = -1;
		}
};

#endif
