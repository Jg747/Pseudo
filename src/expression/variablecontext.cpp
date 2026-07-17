#include "expression/variablecontext.hpp"
#include "components/literals/variable.hpp"

#include <string>
#include <unordered_map>
#include <stdexcept>
#include <memory>

void VariableContext::set(std::shared_ptr<Variable> var) {
    variables[var->get_name()] = var;
}

bool VariableContext::contains(const std::string& name) const {
    return variables.contains(name);
}

std::shared_ptr<Value> VariableContext::get(const std::string& name) const {
    auto it = variables.find(name);

    if (it == variables.end()) {
        throw std::runtime_error("Unknown variable: " + name);
    }

    return it->second->get_value();
}