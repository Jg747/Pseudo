#include <iostream>

#include "interpreter.hpp"
#include "interpreter/analyzers/syntaxanalyzer.hpp"
#include "interpreter/analyzers/programanalyzer.hpp"
#include "interpreter/executor.hpp"

#include "expression/expression.hpp"
#include "expression/variablecontext.hpp"

int main(int argc, char** argv) {
    using namespace std;

    if (argc < 2) {
        cout << "Arguments must be: <file> [<arg1> <arg2> ...]";
        return 1;
    }

    std::ifstream in(argv[1]);
    if (!in) {
        cout << "File not found!";
        return 1;
    }
    
    ProgramAnalyzer f(in);
    try {
        if (!f.analyze()) {
            cout << "Error while parsing";
            return 1;
        }
    } catch (const std::runtime_error& e) {
        cout << e.what();
        return 1;
    }

    in.close();

    int ret;
    try {
        auto e = Executor::new_instance(f.get_funcs(), f.get_entry_point(), f.get_global_vars());
        ret = e.start_pgm(argc, argv);
        cout << "\n[press any key to close]";
        std::getchar();
    } catch (const std::runtime_error& e) {
        cout << "\n" << e.what();
        return 1;
    }

    return ret;
}