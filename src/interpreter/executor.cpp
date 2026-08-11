#include "interpreter/executor.hpp"
#include "expression/expression.hpp"
#include "expression/variablecontext.hpp"
#include "components/instructions/instruction.hpp"
#include "components/literals/variable.hpp"
#include "components/literals/numbervalue.hpp"
#include "interpreter/analyzers/syntaxanalyzer.hpp"
#include "components/literals/function.hpp"
#include "components/literals/specialvalue.hpp"
#include "components/literals/arrayvalue.hpp"

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
    this->global_vars.set(std::make_shared<Variable>(std::string(RETURN_VAR)));

    for (auto& s : global_vars) {
        auto var = std::make_shared<Variable>(s.first);
        this->global_vars.set(var);
        s.second.evaluate(this->global_vars);
    }
}

int Executor::start_pgm(int argc, char** argv) {
    std::vector<std::shared_ptr<Value>> assignations;

    int args = funcs.at(entry)->get_args().size();
    switch (args) {
        case 0:
            break;
        case 1: {
            ArrayValue arr;
            for (int i = 2; i < argc; i++) {
                arr.add_value(std::make_shared<StringValue>(std::string(argv[i])));
            }
            assignations.push_back(arr.clone());
            break;
        }
        default:
            throw std::runtime_error("Entry point syntax is '" + std::string(FUNCTION_STR) + " <entry_func>()' or '" + std::string(FUNCTION_STR) + " <entry_func>(<arr>)'");
    }
    
    auto ret = execute(entry, assignations);

    if (dynamic_cast<NumberValue*>(ret.get())) {
        return static_cast<NumberValue*>(ret.get())->get_int_value();
    }

    if (dynamic_cast<NullValue*>(ret.get())) {
        return 0;
    }

    throw std::runtime_error("Entry point must return integer value");
}

std::shared_ptr<Value> Executor::execute(std::string func_name, std::vector<std::shared_ptr<Value>>& assignations) {
    funcs[func_name]->stack_push();
    auto ret = funcs[func_name]->execute(global_vars, assignations);
    funcs[func_name]->stack_pop();
    return ret;
}

const std::vector<std::string>& Executor::get_params(std::string func_name) const {
    return funcs[func_name]->get_args();
}

bool Executor::test_condition(Expression& condition, VariableContext& scoped_vars) {
    auto res = condition.evaluate(scoped_vars);
    if (dynamic_cast<NumberValue*>(res.get())) {
        return ((NumberValue*) res.get())->get_double_value() != 0;
    }
    throw std::runtime_error("Condition result is not a number (" + res->get_value() + ")");
}