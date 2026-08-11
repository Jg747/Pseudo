#ifndef __VARIABLE_CONTEXT_HPP__
#define __VARIABLE_CONTEXT_HPP__

#include "components/literals/variable.hpp"

#include <string>
#include <unordered_map>
#include <memory>

class VariableContext {
private:
    std::unordered_map<std::string, std::shared_ptr<Variable>> variables;

public:
    VariableContext() {}

    void set(std::shared_ptr<Variable> var);
    bool contains(const std::string& name) const;
    std::shared_ptr<Value> get(const std::string& name) const;
    std::shared_ptr<Variable> get_var(const std::string& name);
    const std::unordered_map<std::string, std::shared_ptr<Variable>>& get_mappings() const;

    VariableContext& operator+=(const VariableContext& other);
    friend VariableContext operator+(const VariableContext& c1, const VariableContext& c2);
};

#endif // __VARIABLE_CONTEXT_HPP__