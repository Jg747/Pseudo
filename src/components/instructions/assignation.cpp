#include "components/instructions/assignation.hpp"
#include "expression/expression.hpp"
#include "expression/variablecontext.hpp"

void Assignation::execute(VariableContext& scoped_vars) {
    // auto ret = e.evaluate(scoped_vars);
    e.evaluate(scoped_vars);
}