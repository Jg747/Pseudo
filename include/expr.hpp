#ifndef __EXPR_HPP__
#define __EXPR_HPP__

#include <vector>
#include <string>

class Expression {
public:
    static bool evaluate(std::vector<std::string>& expression);
};

#endif // __EXPR_HPP__