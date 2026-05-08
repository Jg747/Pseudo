#ifndef __INTERPRETER_HPP__
#define __INTERPRETER_HPP__

#include "components/function.hpp"
#include "components/literals/value.hpp"

#include <string>
#include <unordered_map>
#include <stack>
#include <utility>

namespace interpreter {
    static std::string entry_point;
    static std::unordered_map<std::string, Function> functions;
    static std::unordered_map<std::string, std::unique_ptr<Literal>> global_variables;

    static std::stack<std::pair<Function, std::unique_ptr<Value>>> call_stack;

    bool load_file(std::string filename);
    void execute();
};

#endif // __INTERPRETER_HPP__