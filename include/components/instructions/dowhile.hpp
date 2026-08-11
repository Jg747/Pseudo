#ifndef __DOWHILE_HPP__
#define __DOWHILE_HPP__

#include "loop.hpp"
#include "expression/expression.hpp"
#include "expression/variablecontext.hpp"

class DoWhile : public Loop {
private:
    void abstract() override {}
public:
    DoWhile() {}
    DoWhile(Expression& condition) : Loop(condition) {}
    
    bool execute(VariableContext& scoped_vars) override;
};

#endif // __DOWHILE_HPP__