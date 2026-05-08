#include <iostream>
#include "interpreter.hpp"
#include "analyzers/syntaxanalyzer.hpp"

int main() {
    using namespace std;

    SyntaxAnalyzer an("file.txt");
    if (an.analyze()) {
        cout << "\nOK!";
    } else {
        cout << "\nNON OK!";
    }
    return 0;
}