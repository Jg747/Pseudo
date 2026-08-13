#include "components/instructions/dowhile.hpp"

bool DoWhile::execute(VariableContext& scoped_vars) {
    do {
        for (auto& i : instructions) {
            if (!i->execute(scoped_vars)) {
                return false;
            }
        }
    } while (!test_condition(scoped_vars));
    return true;
}