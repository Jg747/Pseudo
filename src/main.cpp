#include <iostream>

#include "interpreter.hpp"
#include "interpreter/analyzers/syntaxanalyzer.hpp"
#include "interpreter/analyzers/programanalyzer.hpp"
#include "interpreter/executor.hpp"

int main(int argc, char** argv) {
    using namespace std;

    std::ifstream in("file.txt");
    if (!in) {
        cout << "File not found!";
        return 1;
    }
    
    ProgramAnalyzer f(in);
    try {
        if (!f.analyze()) {
            cout << "Errore!";
            return 1;
        }
    } catch (const std::runtime_error& e) {
        cout << e.what();
        return 1;
    }

    in.close();

    try {
        auto e = Executor::new_instance(f.get_funcs(), f.get_entry_point(), f.get_global_vars());
        e.start_pgm();
    } catch (const std::runtime_error& e) {
        cout << e.what();
        return 1;
    }
    
    /*SyntaxAnalyzer an("file.txt");
    if (an.analyze()) {
        cout << "OK!\n--- PGM OUTPUT ---\n";

        Executor e(an.get_instructions(), an.get_var_list());
        e.start_pgm();
    } else {
        cout << "\nNON OK!";
    }*/

    return 0;
}