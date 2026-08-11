#ifndef __READ_HPP__
#define __READ_HPP__

#include "instruction.hpp"
#include "expression/expression.hpp"
#include "expression/variablecontext.hpp"

#include <vector>
#include <string>

class Read : public Instruction {
private:
    std::vector<std::pair<std::string, Expression>> var_order;

public:
    Read(std::vector<std::pair<std::string, Expression>>& var_order) : var_order(var_order) {}
    bool execute(VariableContext& scoped_vars) override;
};

#endif // __READ_HPP__