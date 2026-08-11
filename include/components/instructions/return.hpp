#ifndef __RETURN_HPP__
#define __RETURN_HPP__

#include "instruction.hpp"
#include "expression/expression.hpp"

class Return : public Instruction {
private:
    Expression e;
public:
    Return(Expression& e) : e(e) {}
    bool execute(VariableContext& scoped_vars) override;
};

#endif // __RETURN_HPP__