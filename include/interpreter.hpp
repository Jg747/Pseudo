#ifndef __INTERPRETER_HPP__
#define __INTERPRETER_HPP__

#include "components/function.hpp"
#include "components/literals/value.hpp"

#include <string>
#include <unordered_map>
#include <stack>
#include <utility>

namespace interpreter {
    std::string entry_point;
    std::unordered_map<std::string, Function> functions;
    std::unordered_map<std::string, Literal> global_variables;

    std::stack<std::pair<Function, Value>> call_stack;

    bool load_file(std::string filename);
    void execute();
};

#endif // __INTERPRETER_HPP__