#ifndef __WHILE_HPP__
#define __WHILE_HPP__

#include "loop.hpp"
#include "expression/expression.hpp"
#include "expression/variablecontext.hpp"

class While : public Loop {
private:
    void abstract() override {}
public:
    While(Expression& condition) : Loop(condition) {}
    bool execute(VariableContext& scoped_vars) override;
};

#endif // __WHILE_HPP__