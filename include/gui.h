#ifndef gui
#define gui

#ifdef _WIN32
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif

#include <string>

using std::string;

enum windows {
	SELECT,
	EXECUTE
};

string printHelp(bool easterEgg);

class Gui {
	private:
        // Windows type
		WINDOW *select[2];
		WINDOW *execute[2];

        // Current cursor pos (refreshed every time)
		int cursY;
		int cursX;

        // Easter egg :P
        bool easterEgg;

	public:
        // handle gui object
		Gui();
		~Gui();
		void initGui();
		void setEasterEgg(bool status);
		bool getEasterEgg();

        // init/stop window
		void initScr();
		void interruptWindow();

        // show a window type (setups in gui.cpp)
		void normalSelect();
		void paoloneSelect();
		void selectWindow();
		void fileWindow();
		void helpWindow(string str);
		void executeWindow();
		void errorWindow();

        // gets input from one of the windows
		string getInput(windows w, int size);
		int getWindowChar(windows w);
        void writeInvalid(windows w, string msg, string str);

        // print on window
		void writeOnWindow(windows w, string str);
		void mvwriteOnWindow(windows w, int y, int x, string str);

        // random methods
		void setCursVisible(bool visible);
		void getCurrentXY(int &x, int &y);
		void getWindowSize(int &width, int &height);

        // cmd "cls" [win] or "clear" [unix]
		static void cls();

};

#endif