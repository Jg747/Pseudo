#ifndef __FUNCTION_HPP__
#define __FUNCTION_HPP__

#include "instructions/instruction.hpp"
#include "literals/variable.hpp"

#include <string>
#include <vector>
#include <unordered_map>

class Function {
private:
    std::string name;
    int start;
    int end;
    std::unordered_map<std::string, std::unique_ptr<Variable>> variables;
    std::vector<Instruction> instructions;
    std::vector<Variable> params;

public:
    Function();
    Function(std::vector<Variable>& params);

    void set_params(std::vector<Variable>& params);
    std::vector<Variable>& get_params() const;

    std::vector<Instruction>& get_instructions() const;
    void add_instruction(Instruction& i);

    std::unordered_map<std::string, Variable>& get_variables() const;
    void add_variable(Variable& l);

    void execute();
};

#endif // __FUNCTION_HPP__