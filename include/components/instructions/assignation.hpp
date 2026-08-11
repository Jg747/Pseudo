#ifndef __ASSIGNATION_HPP__
#define __ASSIGNATION_HPP__

#include "instruction.hpp"
#include "expression/expression.hpp"

class Assignation : public Instruction {
private:
    Expression e;
public:
    Assignation(Expression& e) : e(e) {}
    bool execute(VariableContext& scoped_vars) override;
};

#endif // __ASSIGNATION_HPP__