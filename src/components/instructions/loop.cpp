#include "components/instructions/loop.hpp"
#include "expression/expression.hpp"
#include "expression/variablecontext.hpp"
#include "components/literals/numbervalue.hpp"
#include "interpreter/executor.hpp"

void Loop::set_condition(Expression& condition) {
    this->condition = condition;
}

bool Loop::test_condition(VariableContext& scoped_vars) {
    return Executor::test_condition(condition, scoped_vars);
}