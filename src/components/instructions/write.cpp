#include "components/instructions/write.hpp"
#include "expression/expression.hpp"
#include "expression/variablecontext.hpp"
#include "components/literals/arrayvalue.hpp"
#include "components/literals/numbervalue.hpp"

#include <iostream>

std::string WriteExpr::print(VariableContext& vars) {
    auto val = e.evaluate(vars);
    std::string ret;
    if (dynamic_cast<ArrayValue*>(val.get())) {
        ArrayValue* arr = (ArrayValue*) val.get();
        ret += "[";
        for (int i = 0; i < arr->get_length(); i++) {
            ret += (*arr)[i]->get_value() + ", ";
        }
        ret = ret.substr(0, ret.find_last_of(", ")) + "]";
    } else if (dynamic_cast<NumberValue*>(val.get())) {
        ret = ((NumberValue*) val.get())->get_value();
    } else {
        ret = val->get_value();
    }
    return ret;
}

void Write::execute(VariableContext& scoped_vars) {
    for (auto& p : print_order) {
        std::cout << p->print(scoped_vars);
    }
}