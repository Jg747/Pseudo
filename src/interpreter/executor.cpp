#include "interpreter/executor.hpp"
#include "components/instructions/instruction.hpp"
#include "components/literals/variable.hpp"

#include <string>
#include <vector>
#include <list>
#include <memory>

Executor::Executor(std::list<std::unique_ptr<Instruction>>& list, std::vector<std::string> var_list) : list(std::move(list)) {
    for (auto& s : var_list) {
        scoped_vars.set(std::make_shared<Variable>(s));
    }
}

void Executor::start_pgm() {
    for (auto& i : list) {
        i->execute(scoped_vars);
    }
}