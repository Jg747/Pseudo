#ifndef __EXPR_HPP__
#define __EXPR_HPP__

#include "components/literals/value.hpp"

#include <vector>
#include <string>

class Expression {
public:
    static bool evaluate(std::vector<std::string>& expression);
    static Value& calculate();
};

#endif // __EXPR_HPP__