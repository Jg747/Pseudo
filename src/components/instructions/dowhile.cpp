#include "components/instructions/dowhile.hpp"

void DoWhile::execute(VariableContext& scoped_vars) {
    do {
        for (auto& i : instructions) {
            i->execute(scoped_vars);
        }
    } while (test_condition(scoped_vars));
}