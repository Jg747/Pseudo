#ifndef __READ_HPP__
#define __READ_HPP__

#include "instruction.hpp"
#include "expression/variablecontext.hpp"

#include <vector>
#include <string>

class Read : public Instruction {
private:
    std::vector<std::string> var_order;

public:
    Read(std::vector<std::string>& var_order) : var_order(var_order) {}
    void execute(VariableContext& scoped_vars) override;
};

#endif // __READ_HPP__