#include "interpreter.hpp"
#include "components/function.hpp"
#include "components/literals/value.hpp"
#include "analyzers/syntaxanalyzer.hpp"
#include "analyzers/literalanalyzer.hpp"
#include "analyzers/functionanalyzer.hpp"

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