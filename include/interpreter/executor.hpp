#ifndef __EXECUTOR_HPP__
#define __EXECUTOR_HPP__

#include "components/instructions/instruction.hpp"
#include "components/literals/variable.hpp"
#include "components/literals/function.hpp"
#include "expression/expression.hpp"
#include "expression/variablecontext.hpp"

#include <string>
#include <list>
#include <unordered_map>
#include <memory>
#include <vector>

class Executor {
private:
    static inline std::unique_ptr<Executor> e = nullptr;

    std::unordered_map<std::string, std::unique_ptr<Function>>& funcs;
    std::string entry;
    VariableContext global_vars;

    void set_global_vars(std::vector<std::pair<std::string, Expression>>& global_vars);

public:
    static Executor& new_instance(std::unordered_map<std::string, std::unique_ptr<Function>>&& funcs, std::string entry, std::vector<std::pair<std::string, Expression>>& global_vars);
    static Executor& get_instance();
    static bool test_condition(Expression& condition, VariableContext& scoped_vars);
    
    Executor(std::unordered_map<std::string, std::unique_ptr<Function>>& funcs, std::string entry, std::vector<std::pair<std::string, Expression>>& global_vars);
    int start_pgm(int argc, char** argv);
    std::shared_ptr<Value> execute(std::string func_name, std::vector<std::shared_ptr<Value>>& assignations);
    const std::vector<std::string>& get_params(std::string func_name) const;
};

#endif /* __EXECUTOR_HPP__ */