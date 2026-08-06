#include "interpreter/executor.hpp"
#include "expression/expression.hpp"
#include "expression/variablecontext.hpp"
#include "components/instructions/instruction.hpp"
#include "components/literals/variable.hpp"
#include "components/literals/numbervalue.hpp"
#include "interpreter/analyzers/syntaxanalyzer.hpp"

#include <string>
#include <vector>
#include <list>
#include <memory>

Executor::Executor(std::list<std::unique_ptr<Instruction>>& list, std::vector<std::string> var_list) : list(std::move(list)) {
    for (auto& s : var_list) {
        scoped_vars.set(std::make_shared<Variable>(s));
    }
    scoped_vars.set(std::make_shared<Variable>(std::string(READ_VAR)));
}

void Executor::start_pgm() {
    for (auto& i : list) {
        i->execute(scoped_vars);
    }
}

bool Executor::test_condition(Expression& condition, VariableContext& scoped_vars) {
    auto res = condition.evaluate(scoped_vars);
    if (dynamic_cast<NumberValue*>(res.get())) {
        return ((NumberValue*) res.get())->get_double_value() != 0;
    }
    throw std::runtime_error("Condition result is not a number (" + res->get_value() + ")");
}