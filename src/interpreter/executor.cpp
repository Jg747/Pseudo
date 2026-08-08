#include "interpreter/executor.hpp"
#include "expression/expression.hpp"
#include "expression/variablecontext.hpp"
#include "components/instructions/instruction.hpp"
#include "components/literals/variable.hpp"
#include "components/literals/numbervalue.hpp"
#include "interpreter/analyzers/syntaxanalyzer.hpp"
#include "components/literals/function.hpp"

#include <string>
#include <vector>
#include <list>
#include <memory>

Executor& Executor::new_instance(std::unordered_map<std::string, std::unique_ptr<Function>>&& funcs, std::string entry, std::vector<std::pair<std::string, Expression>>& global_vars) {
    e = std::make_unique<Executor>(funcs, entry, global_vars);
    return *e;
}

Executor& Executor::get_instance() {
    if (e == nullptr) {
        throw std::runtime_error("Instance must be created first");
    }
    return *e;
}

Executor::Executor(std::unordered_map<std::string, std::unique_ptr<Function>>& funcs, std::string entry, std::vector<std::pair<std::string, Expression>>& global_vars) : funcs(funcs), entry(entry) {
    set_global_vars(global_vars);
}

void Executor::set_global_vars(std::vector<std::pair<std::string, Expression>>& global_vars) {
    this->global_vars.set(std::make_shared<Variable>(std::string(READ_VAR)));

    for (auto& s : global_vars) {
        auto var = std::make_shared<Variable>(s.first);
        this->global_vars.set(var);
        s.second.evaluate(this->global_vars);
    }
}

void Executor::start_pgm() {
    std::vector<Expression> assignations;
    funcs[entry]->execute(global_vars, assignations);
}

void Executor::execute(std::string func_name, std::vector<Expression>& assignations) {
    //funcs[func_name].execute(global_vars);
}

bool Executor::test_condition(Expression& condition, VariableContext& scoped_vars) {
    auto res = condition.evaluate(scoped_vars);
    if (dynamic_cast<NumberValue*>(res.get())) {
        return ((NumberValue*) res.get())->get_double_value() != 0;
    }
    throw std::runtime_error("Condition result is not a number (" + res->get_value() + ")");
}