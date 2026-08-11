#include "components/literals/function.hpp"
#include "interpreter/analyzers/syntaxanalyzer.hpp"

#include <memory>

Function::Function(std::string& name, std::vector<std::string>& args) : name(name), args(args) {
    for (auto& a : args) {
        this->scoped_vars.set(std::make_shared<Variable>(a));
    }
}

std::string Function::get_name() {
    return name;
}

std::shared_ptr<Value> Function::execute(VariableContext& global_vars, std::vector<std::shared_ptr<Value>>& assignations) {
    VariableContext context = scoped_vars + global_vars;

    if (assignations.size() != args.size()) {
        throw std::runtime_error("Invalid arguments provided to function '" + name + "'");
    }

    for (size_t i = 0; i < args.size(); i++) {
        context.get_var(args[i])->set_value(assignations[assignations.size() - 1 - i]);
    }

    for (auto& i : list) {
        if (!i->execute(context)) {
            return global_vars.get(RETURN_VAR)->clone();
        }
    }

    return global_vars.get(RETURN_VAR)->clone();
}

const std::vector<std::string>& Function::get_args() const {
    return args;
}

void Function::set_instructions(std::list<std::unique_ptr<Instruction>>& list) {
    this->list = std::move(list);
}

void Function::set_vars(std::vector<std::string>& vars) {
    for (auto& v : vars) {
        this->scoped_vars.set(std::make_shared<Variable>(v));
    }
}

void Function::stack_push() {
    std::vector<std::shared_ptr<Value>> values;

    for (auto& [name, var] : scoped_vars.get_mappings()) {
        values.push_back(var->get_value());
    }

    values_stack.push(values);
}

void Function::stack_pop() {
    std::vector<std::shared_ptr<Value>> values = values_stack.top();
    values_stack.pop();

    int i = 0;
    for (auto& [name, var] : scoped_vars.get_mappings()) {
        var->set_value(values[i]);
        i++;
    }
}