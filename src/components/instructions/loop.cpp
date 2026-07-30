#include "components/instructions/loop.hpp"
#include "expression/expression.hpp"
#include "expression/variablecontext.hpp"
#include "components/literals/numbervalue.hpp"

void Loop::set_condition(Expression& condition) {
    this->condition = condition;
}

bool Loop::test_condition(VariableContext& scoped_vars) const {
    auto res = condition.evaluate(scoped_vars);
    if (dynamic_cast<NumberValue*>(res.get())) {
        return ((NumberValue*) res.get())->get_double_value() != 0;
    }
    throw std::runtime_error("Condition result is not a number (" + res->get_value() + ")");
}