#ifndef __EXECUTOR_HPP__
#define __EXECUTOR_HPP__

#include "components/instructions/instruction.hpp"
#include "components/literals/variable.hpp"

#include <string>
#include <list>
#include <unordered_map>
#include <memory>
#include <vector>

class Executor {
private:
    // TODO quando implementate le Functions spostare le seguenti proprietà per le funzioni
    std::list<std::unique_ptr<Instruction>> list;
    VariableContext scoped_vars;
public:
    Executor(std::list<std::unique_ptr<Instruction>>& list, std::vector<std::string> var_list);
    void start_pgm();
};

#endif /* __EXECUTOR_HPP__ */