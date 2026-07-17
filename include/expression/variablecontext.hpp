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
    void set(std::shared_ptr<Variable> var);
    bool contains(const std::string& name) const;
    std::shared_ptr<Value> get(const std::string& name) const;
};

#endif // __VARIABLE_CONTEXT_HPP__