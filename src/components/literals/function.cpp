#include "components/literals/function.hpp"

Function::Function(std::string& name, std::vector<std::string>& args) : name(name), args(args) {
    merged_vars = false;

    for (auto& a : args) {
        this->scoped_vars.set(std::make_shared<Variable>(a));
    }
}

std::string Function::get_name() {
    return name;
}

void Function::execute(VariableContext& global_vars, std::vector<Expression>& assignations) {
    if (!merged_vars) {
        merged_vars = true;
        scoped_vars += global_vars;
    }

    for (auto& e : assignations) {
        e.evaluate(scoped_vars);
    }

    for (auto& i : list) {
        i->execute(scoped_vars);
    }
}

void Function::set_instructions(std::list<std::unique_ptr<Instruction>>& list) {
    this->list = std::move(list);
}

void Function::set_vars(std::vector<std::string>& vars) {
    for (auto& v : vars) {
        this->scoped_vars.set(std::make_shared<Variable>(v));
    }
}