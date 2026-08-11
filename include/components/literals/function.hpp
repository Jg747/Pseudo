#ifndef __FUNCTION_HPP__
#define __FUNCTION_HPP__

#include "components/instructions/instruction.hpp"
#include "components/literals/variable.hpp"
#include "expression/expression.hpp"
#include "components/literals/value.hpp"

#include <string>
#include <vector>
#include <unordered_map>
#include <list>
#include <memory>

class Function {
private:
    std::string name;
    std::vector<std::string> args;

    VariableContext scoped_vars;
    std::list<std::unique_ptr<Instruction>> list;

    std::stack<std::vector<std::shared_ptr<Value>>> values_stack;

public:
    Function(std::string& name, std::vector<std::string>& args);
    void set_instructions(std::list<std::unique_ptr<Instruction>>& list);
    void set_vars(std::vector<std::string>& vars);
    const std::vector<std::string>& get_args() const;

    std::string get_name();

    void stack_push();
    void stack_pop();

    std::shared_ptr<Value> execute(VariableContext& global_vars, std::vector<std::shared_ptr<Value>>& assignations);
};

#endif // __FUNCTION_HPP__