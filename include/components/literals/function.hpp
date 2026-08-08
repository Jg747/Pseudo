#ifndef __FUNCTION_HPP__
#define __FUNCTION_HPP__

#include "components/instructions/instruction.hpp"
#include "components/literals/variable.hpp"
#include "expression/expression.hpp"

#include <string>
#include <vector>
#include <unordered_map>
#include <list>

class Function {
private:
    std::string name;
    std::vector<std::string> args;

    VariableContext scoped_vars;
    std::list<std::unique_ptr<Instruction>> list;
    
    bool merged_vars;

public:
    Function(std::string& name, std::vector<std::string>& args);
    void set_instructions(std::list<std::unique_ptr<Instruction>>& list);
    void set_vars(std::vector<std::string>& vars);

    std::string get_name();

    void execute(VariableContext& global_vars, std::vector<Expression>& assignations);
};

#endif // __FUNCTION_HPP__