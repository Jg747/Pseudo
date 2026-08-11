#include "components/instructions/while.hpp"

bool While::execute(VariableContext& scoped_vars) {
    while (test_condition(scoped_vars)) {
        for (auto& i : instructions) {
            if (!i->execute(scoped_vars)) {
                return false;
            }
        }
    }
    return true;
}