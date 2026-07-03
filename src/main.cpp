#include <iostream>
#include "interpreter.hpp"
#include "analyzers/syntaxanalyzer.hpp"
#include "components/literals/value.hpp"
#include "components/literals/numbervalue.hpp"
#include "components/literals/stringvalue.hpp"
#include "components/literals/variable.hpp"

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