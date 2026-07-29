#include "components/instructions/read.hpp"
#include "expression/variablecontext.hpp"
#include "components/literals/numbervalue.hpp"
#include "components/literals/stringvalue.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <exception>

void Read::execute(VariableContext& scoped_vars) {
    for (auto& var : var_order) {
        std::string in;
        std::getline(std::cin, in);
        try {
            NumberValue v(in);
            scoped_vars.get_var(var)->set_value(v);
        } catch (std::runtime_error& e) {
            StringValue s(in);
            scoped_vars.get_var(var)->set_value(s);
        }
    }
}