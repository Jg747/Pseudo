#include "components/instructions/call.hpp"
#include "expression/expression.hpp"
#include "expression/variablecontext.hpp"
#include "components/literals/specialvalue.hpp"
#include "interpreter/analyzers/syntaxanalyzer.hpp"

bool Call::execute(VariableContext& scoped_vars) {
    e.evaluate(scoped_vars);
    return true;
}