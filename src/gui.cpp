#ifdef _WIN32
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif

#include <string>
#include <iostream>

#include <gui.h>
#include <lang.h>

using std::string;

Gui::Gui() {
	initGui();
}

Gui::~Gui() {
	clear();
	endwin();
	std::cout << "\n\n\n\n";
}

void Gui::initGui() {
	this->easterEgg = false;
	
	initscr();

	curs_set(0);

	//Border
	select[1] = newwin(LINES, COLS, 0, 0);
	box(select[1], ACS_VLINE, ACS_HLINE);

	execute[1] = newwin(LINES, COLS, 0, 0);
	box(execute[1], ACS_VLINE, ACS_HLINE);

	//Title
	select[0] = newwin(LINES-2, COLS-2, 1, 1);
	
	//Execution
	execute[0] = newwin(LINES-2, COLS-2, 1, 1);
	scrollok(execute[0], TRUE);

	refresh();
	wrefresh(execute[1]);
	wrefresh(execute[0]);
	wrefresh(select[1]);
	wrefresh(select[0]);
}

void Gui::initScr() {
	initscr();

	//Border
	execute[1] = newwin(LINES, COLS, 0, 0);
	box(execute[1], ACS_VLINE, ACS_HLINE);

	//Execution
	execute[0] = newwin(LINES-2, COLS-2, 1, 1);
	scrollok(execute[0], TRUE);

	refresh();
	wrefresh(execute[1]);
	wrefresh(execute[0]);
}

void Gui::setEasterEgg(bool state) {
	this->easterEgg = state;
}

bool Gui::getEasterEgg() {
	return this->easterEgg;
}

void Gui::selectWindow() {
	refresh();
	box(select[1], ACS_VLINE, ACS_HLINE);
	
	if (this->easterEgg) {
		paoloneSelect();
	} else {
		normalSelect();
	}

	getyx(select[0], cursY, cursX);
	wrefresh(select[1]);
	wrefresh(select[0]);
}

void Gui::paoloneSelect() {
	mvwprintw(select[0], (LINES-2)/4, ((COLS-2)/2)-(string(P_TITLE_NCUR).length()/2), "%s", P_TITLE_NCUR);
	mvwprintw(select[0], ((LINES-2)/2)-3, ((COLS-2)/2)-10, "1. Interprete");
	mvwprintw(select[0], ((LINES-2)/2)-2, ((COLS-2)/2)-10, "2. C++ -> Pseudo");
	mvwprintw(select[0], ((LINES-2)/2)-1, ((COLS-2)/2)-10, "3. Pseudo -> C++");
	mvwprintw(select[0], ((LINES-2)/2), ((COLS-2)/2)-10, "4. Aiuto");
	mvwprintw(select[0], ((LINES-2)/2)+1, ((COLS-2)/2)-10, "0. Esci");
	mvwprintw(select[0], ((LINES-2)/2)+3, ((COLS-2)/2)-10, "Scelta: ");
}

void Gui::normalSelect() {
	mvwprintw(select[0], (LINES-2)/4, ((COLS-2)/2)-(string(TITLE_NCUR).length()/2), "%s", TITLE_NCUR);
	mvwprintw(select[0], ((LINES-2)/2)-3, ((COLS-2)/2)-10, "1. Interpreter");
	mvwprintw(select[0], ((LINES-2)/2)-2, ((COLS-2)/2)-10, "2. C++ -> Pseudo");
	mvwprintw(select[0], ((LINES-2)/2)-1, ((COLS-2)/2)-10, "3. Pseudo -> C++");
	mvwprintw(select[0], ((LINES-2)/2), ((COLS-2)/2)-10, "4. Help");
	mvwprintw(select[0], ((LINES-2)/2)+1, ((COLS-2)/2)-10, "0. Quit");
	mvwprintw(select[0], ((LINES-2)/2)+3, ((COLS-2)/2)-10, "Choice: ");
}

void Gui::fileWindow() {
	refresh();
	box(select[1], ACS_VLINE, ACS_HLINE);
	mvwprintw(select[0], (LINES-2)/4, ((COLS-2)/2)-(string(TITLE_NCUR).length()/2), "%s", TITLE_NCUR);
	mvwprintw(select[0], ((LINES-2)/2)-3, ((COLS-2)/2)-20, "File (0 back): ");
	getyx(select[0], cursY, cursX);
	wrefresh(select[1]);
	wrefresh(select[0]);
}

void Gui::helpWindow(string str) {
	endwin();
	std::cout << str;
	getchar();
	cls();
	initGui();
}

void Gui::executeWindow() {
	refresh();
	box(execute[1], ACS_VLINE, ACS_HLINE);
	wclear(execute[0]);
	wrefresh(execute[1]);
	wrefresh(execute[0]);
}

void Gui::errorWindow() {
	refresh();
	box(execute[1], ACS_VLINE, ACS_HLINE);
	wclear(execute[0]);
	mvwprintw(execute[0], 0, ((COLS-2)/2)-3, this->easterEgg ? P_ERRORS : ERRORS);
	wrefresh(execute[1]);
	wrefresh(execute[0]);
}

void Gui::writeInvalid(windows w, string msg, string str) {
	switch (w) {
		case SELECT:
			wmove(select[0], cursY, cursX + str.length());
			wprintw(select[0], msg.c_str());
			wrefresh(select[0]);
			break;
		case EXECUTE:
			wmove(execute[0], cursY, cursX + str.length());
			wprintw(execute[0], msg.c_str());
			wrefresh(execute[0]);
			break;
	}
}

string Gui::getInput(windows w, int size) {
	curs_set(1);
	char buff[size];
	switch (w) {
		case SELECT:
			wgetnstr(select[0], buff, size);
			break;
		case EXECUTE:
			wgetnstr(execute[0], buff, size);
			break;
	}
	curs_set(0);
	return string(buff);
}

int Gui::getWindowChar(windows w) {
	switch (w) {
		case SELECT:
			return wgetch(select[0]);
			break;
		case EXECUTE:
			return wgetch(execute[0]);
			break;
	}
}

void Gui::writeOnWindow(windows w, string str) {
	switch (w) {
		case SELECT:
			wprintw(select[0], "%s", str.c_str());
			wrefresh(select[0]);
			break;
		case EXECUTE:
			wprintw(execute[0], "%s", str.c_str());
			wrefresh(execute[0]);
			break;
	}
}

void Gui::mvwriteOnWindow(windows w, int y, int x, string str) {
	switch (w) {
		case SELECT:
			wmove(select[0], y, x);
			wprintw(select[0], "%s", str.c_str());
			break;
		case EXECUTE:
			wmove(execute[0], y, x);
			wprintw(execute[0], "%s", str.c_str());
			break;
	}
}

void Gui::setCursVisible(bool visible) {
	if (visible) {
		curs_set(1);
	} else {
		curs_set(0);
	}
}

void Gui::getCurrentXY(int &x, int &y) {
	y = cursY;
	x = cursX;
}

void Gui::getWindowSize(int &width, int &height) {
	width = COLS;
	height = LINES;
}

void Gui::interruptWindow() {
	endwin();
}

#ifdef _WIN32
void Gui::cls() {
	system("cls");
}
#else
void Gui::cls() {
	system("clear");
}
#endif

string printHelp(bool easterEgg) {
	if (easterEgg) {
		return 
			string(P_TITLE)
			+ "COMANDI\n\n"
			+ "-f [fileName]\tFile .txt o .cpp\n"
			+ "-e\tEsegui lo pseudocodice [solo txt]\n"
			+ "-t\tTraduce il file nell'altro linguaggio (.txt -> .cpp | .cpp -> .txt)\n"
			+ "-nogui\tEsegue lo pseudocodice senza GUI\n"
			+ "-help\tStampa questa pagina\n"
			+ "Esempio di utilizzo con i comandi: \n"
			+ "Recarsi nella cartella: pseudo -f test.txt -e -nogui\n\n\n"
			+ "AIUTO NELLA SINTASSI\n\n"
			+ "\tDichiarazione variabile: leggi <var> | <var> <- <valore>\n"
			+ "\tInserimento valore in una variabile: <var> <- <espressione>\n"
			+ "\tInput: leggi <var> | leggi <var>, <var>, ...\n"
			+ "\tOutput: scrivi \"<testo>\" | scrivi <var> | scrivi \"<testo>\", <var>, ...\n\n"
			+ "\tIF (unario):\n"
			+ "\t\tSE (<condizione>)\n"
			+ "\t\t\tALLORA\n"
			+ "\t\t\t\tINIZIO\n"
			+ "\t\t\t\t\t<istruzioni>\n"
			+ "\t\t\t\tFINE\n"
			+ "\t\tFINE_SE\n\n"
			+ "\tIF (binario):\n"
			+ "\t\tSE (<condizione>)\n"
			+ "\t\t\tALLORA\n"
			+ "\t\t\t\tINIZIO\n"
			+ "\t\t\t\t\t<istruzioni>\n"
			+ "\t\t\t\tFINE\n"
			+ "\t\t\tALTRIMENTI\n"
			+ "\t\t\t\tINIZIO\n"
			+ "\t\t\t\t\t<istruzioni>\n"
			+ "\t\t\t\tFINE\n"
			+ "\t\tFINE_SE\n\n"
			+ "\tWHILE:\n"
			+ "\t\tFINTANTOCHE' (<condizione>) ESEGUI\n"
			+ "\t\t\tINIZIO\n"
			+ "\t\t\t\t<istruzioni>\n"
			+ "\t\t\tFINE\n\n"
			+ "\tDO WHILE:\n"
			+ "\t\tRIPETI\n"
			+ "\t\t\t<istruzioni>\n"
			+ "\t\tFINO_A_CHE (<condizione>)\n\n"
			+ string(P_FOOTER);
	} else {
		return 
			string(TITLE)
			+ "COMANDI\n\n"
			+ "-f [fileName]\tFile .txt o .cpp\n"
			+ "-e\tEsegui lo pseudocodice [solo txt]\n"
			+ "-t\tTraduce il file nell'altro linguaggio (.txt -> .cpp | .cpp -> .txt)\n"
			+ "-nogui\tEsegue lo pseudocodice senza GUI\n"
			+ "-help\tStampa questa pagina\n"
			+ "Esempio di utilizzo con i comandi: \n"
			+ "Recarsi nella cartella: pseudo -f test.txt -e -nogui\n\n\n"
			+ "AIUTO NELLA SINTASSI\n\n"
			+ "\tDichiarazione variabile: leggi <var> | <var> <- <valore>\n"
			+ "\tInserimento valore in una variabile: <var> <- <espressione>\n"
			+ "\tInput: leggi <var> | leggi <var>, <var>, ...\n"
			+ "\tOutput: scrivi \"<testo>\" | scrivi <var> | scrivi \"<testo>\", <var>, ...\n\n"
			+ "\tIF (unario):\n"
			+ "\t\tSE (<condizione>)\n"
			+ "\t\t\tALLORA\n"
			+ "\t\t\t\tINIZIO\n"
			+ "\t\t\t\t\t<istruzioni>\n"
			+ "\t\t\t\tFINE\n"
			+ "\t\tFINE_SE\n\n"
			+ "\tIF (binario):\n"
			+ "\t\tSE (<condizione>)\n"
			+ "\t\t\tALLORA\n"
			+ "\t\t\t\tINIZIO\n"
			+ "\t\t\t\t\t<istruzioni>\n"
			+ "\t\t\t\tFINE\n"
			+ "\t\t\tALTRIMENTI\n"
			+ "\t\t\t\tINIZIO\n"
			+ "\t\t\t\t\t<istruzioni>\n"
			+ "\t\t\t\tFINE\n"
			+ "\t\tFINE_SE\n\n"
			+ "\tWHILE:\n"
			+ "\t\tFINTANTOCHE' (<condizione>) ESEGUI\n"
			+ "\t\t\tINIZIO\n"
			+ "\t\t\t\t<istruzioni>\n"
			+ "\t\t\tFINE\n\n"
			+ "\tDO WHILE:\n"
			+ "\t\tRIPETI\n"
			+ "\t\t\t<istruzioni>\n"
			+ "\t\tFINO_A_CHE (<condizione>)\n\n"
			+ string(FOOTER);
	}
}