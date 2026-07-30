#include "components/instructions/while.hpp"

void While::execute(VariableContext& scoped_vars) {
    while (test_condition(scoped_vars)) {
        for (auto& i : instructions) {
            i->execute(scoped_vars);
        }
    }
}