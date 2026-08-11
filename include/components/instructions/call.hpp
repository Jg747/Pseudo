#ifndef __CALL_HPP__
#define __CALL_HPP__

#include "instruction.hpp"
#include "expression/expression.hpp"

class Call : public Instruction {
private:
    Expression e;
public:
    Call(Expression& e) : e(e) {}
    bool execute(VariableContext& scoped_vars) override;
};

#endif // __CALL_HPP__