#include <iostream>

#include "interpreter.hpp"
#include "interpreter/analyzers/syntaxanalyzer.hpp"
#include "components/literals/value.hpp"
#include "components/literals/numbervalue.hpp"
#include "components/literals/stringvalue.hpp"
#include "components/literals/arrayvalue.hpp"
#include "components/literals/variable.hpp"
#include "expression/lexer.hpp"
#include "expression/expression.hpp"
#include "expression/parser.hpp"
#include "expression/token.hpp"
#include "expression/variablecontext.hpp"
#include "interpreter/executor.hpp"

int main(int argc, char** argv) {
    using namespace std;

    SyntaxAnalyzer an("file.txt");
    if (an.analyze()) {
        cout << "OK!\n--- PGM OUTPUT ---\n";

        Executor e(an.get_instructions(), an.get_var_list());
        e.start_pgm();
    } else {
        cout << "\nNON OK!";
    }

    return 0;
}