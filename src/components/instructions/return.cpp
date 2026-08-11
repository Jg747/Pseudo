#include "components/instructions/return.hpp"
#include "expression/expression.hpp"
#include "expression/variablecontext.hpp"
#include "components/literals/specialvalue.hpp"
#include "interpreter/analyzers/syntaxanalyzer.hpp"

bool Return::execute(VariableContext& scoped_vars) {
    auto ret = e.evaluate(scoped_vars);
    scoped_vars.get_var(RETURN_VAR)->set_value(ret);
    return false;
}