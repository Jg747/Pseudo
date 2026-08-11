#ifndef __IF_HPP__
#define __IF_HPP__

#include "scopedinstruction.hpp"
#include "instruction.hpp"
#include "expression/expression.hpp"
#include "expression/variablecontext.hpp"

#include <list>
#include <vector>

struct sequence {
    Expression condition;
    std::list<std::unique_ptr<Instruction>> instructions;

    bool test_condition(VariableContext& scoped_vars);
};

class If : public ScopedInstruction {
private:
    std::vector<sequence> cases;

    void abstract() override {}

public:
    If() {}
    
    void add_sequence(Expression& condition);
    void add_instruction(std::unique_ptr<Instruction>& i) override;
    Instruction* get_last_instruction();

    bool execute(VariableContext& scoped_vars) override;
};

#endif // __IF_HPP__