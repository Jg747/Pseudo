#include "interpreter.hpp"
#include "components/literals/function.hpp"
#include "components/literals/value.hpp"
#include "interpreter/analyzers/syntaxanalyzer.hpp"
#include "interpreter/analyzers/literalanalyzer.hpp"
#include "interpreter/analyzers/functionanalyzer.hpp"

#include <string>
#include <unordered_map>
#include <stack>
#include <utility>

namespace interpreter {
    bool load_file(std::string filename) {
        SyntaxAnalyzer syntax(filename);
        return true;
    }
};