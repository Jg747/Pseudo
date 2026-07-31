#include "components/instructions/if.hpp"
#include "expression/expression.hpp"
#include "expression/variablecontext.hpp"
#include "components/literals/numbervalue.hpp"
#include "interpreter/executor.hpp"

#include <list>
#include <memory>
#include <vector>

bool sequence::test_condition(VariableContext& scoped_vars) {
    return Executor::test_condition(condition, scoped_vars);
}

void If::add_sequence(Expression& condition) {
    cases.push_back(sequence());
    cases.back().condition = condition;
}

void If::add_instruction(std::unique_ptr<Instruction>& i) {
    this->cases.back().instructions.push_back(std::move(i));
}

Instruction* If::get_last_instruction() {
    return cases.back().instructions.back().get();
}

void If::execute(VariableContext& scoped_vars) {
    for (auto& s : cases) {
        if (s.test_condition(scoped_vars)) {
            for (auto& i : s.instructions) {
                i->execute(scoped_vars);
            }
            break;
        }
    }
}