#include "components/instructions/read.hpp"
#include "expression/variablecontext.hpp"
#include "components/literals/numbervalue.hpp"
#include "components/literals/stringvalue.hpp"
#include "interpreter/analyzers/syntaxanalyzer.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <exception>

bool Read::execute(VariableContext& scoped_vars) {
    for (auto& var : var_order) {
        std::string in;
        std::getline(std::cin, in);
        try {
            NumberValue v(in);
            scoped_vars.get_var(READ_VAR)->set_value(v);
        } catch (const std::runtime_error& e) {
            StringValue s(in);
            scoped_vars.get_var(READ_VAR)->set_value(s);
        }
        var.second.evaluate(scoped_vars);
    }
    return true;
}