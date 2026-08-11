#include "expression/variablecontext.hpp"
#include "components/literals/variable.hpp"
#include "components/literals/specialvalue.hpp"

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

std::shared_ptr<Variable> VariableContext::get_var(const std::string& name) {
    auto it = variables.find(name);

    if (it == variables.end()) {
        throw std::runtime_error("Unknown variable: " + name);
    }

    return it->second;
}

VariableContext& VariableContext::operator+=(const VariableContext& other) {
    for (auto [name, pointer] : other.variables) {
        this->variables[name] = pointer;
    }
    return *this;
}

const std::unordered_map<std::string, std::shared_ptr<Variable>>& VariableContext::get_mappings() const {
    return variables;
}

VariableContext operator+(const VariableContext& c1, const VariableContext& c2) {
    VariableContext ret;
    ret += c1;
    ret += c2;
    return ret;
}