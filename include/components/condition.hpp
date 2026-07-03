#ifndef __CONDITION_HPP__
#define __CONDITION_HPP__

#include "components/literals/variable.hpp"

#include <string>
#include <unordered_map>

// Dijkstra shunting-yard algorithm

class Condition {
private:
    std::string expression;
    std::unordered_map<std::string, Variable> scope_variables;
public:
    Condition(std::string expression, const std::unordered_map<std::string, Variable>& scope_variables);
    bool test_condition() const;

    operator bool() const;
};

#endif // __CONDITION_HPP__