#ifndef __FUNCTION_HPP__
#define __FUNCTION_HPP__

#include "instructions/instruction.hpp"
#include "literals/literal.hpp"

#include <string>
#include <vector>
#include <unordered_map>

class Function {
private:
    std::string name;
    int start;
    int end;
    std::unordered_map<std::string, Literal> variables;
    std::vector<Instruction> instructions;
    std::vector<Literal> params;

public:
    Function();
    Function(std::vector<Literal>& params);

    void set_params(std::vector<Literal>& params);
    std::vector<Literal>& get_params() const;

    std::vector<Instruction>& get_instructions() const;
    void add_instruction(Instruction& i);

    std::unordered_map<std::string, Literal>& get_variables() const;
    void add_variable(Literal& l);

    void execute();
};

#endif // __FUNCTION_HPP__