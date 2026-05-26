#include "expr.hpp"

#include <vector>
#include <string>
// TODO
bool Expression::evaluate(std::vector<std::string>& expression) {
    for (const auto& s : expression) {
        printf("'%s' ", s.c_str());
    }
    printf("\n");
    return true;
}