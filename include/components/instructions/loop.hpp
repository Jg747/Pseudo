#ifndef __LOOP_HPP__
#define __LOOP_HPP__

#include "scopedinstruction.hpp"
#include "expression/expression.hpp"
#include "expression/variablecontext.hpp"

#include <vector>

class Loop : public ScopedInstruction {
protected:
    Expression condition;

public:
    Loop() {}
    Loop(Expression& condition) : condition(condition) {}

    void set_condition(Expression& condition);
    bool test_condition(VariableContext& scoped_vars) const;
};

#endif // __LOOP_HPP__