#include "components/instructions/write.hpp"
#include "expression/expression.hpp"
#include "expression/variablecontext.hpp"
#include "components/literals/arrayvalue.hpp"
#include "components/literals/numbervalue.hpp"
#include "components/literals/specialvalue.hpp"

#include <iostream>

static std::string add_arr(ArrayValue* arr) {
    std::string ret = "[";
    for (int i = 0; i < arr->get_length(); i++) {
        if (dynamic_cast<NumberValue*>((*arr)[i].get())) {
            ret += ((NumberValue*) (*arr)[i].get())->get_value();
        } else if (dynamic_cast<ArrayValue*>((*arr)[i].get())) {
            ret += add_arr((ArrayValue*) (*arr)[i].get());
        } else {
            ret += "'" + (*arr)[i]->get_value() + "'";
        }
        ret += ", ";
    }
    ret = ret.substr(0, ret.find_last_of(", ") - 1) + "]";
    return ret;
}

std::string WriteExpr::print(VariableContext& vars) {
    auto val = e.evaluate(vars);
    if (dynamic_cast<NullValue*>(val.get())) {
        throw std::runtime_error("Variable '" + val->get_variable()->get_name() + "' not declared");
    }

    std::string ret;
    if (dynamic_cast<ArrayValue*>(val.get())) {
        ret = add_arr((ArrayValue*) val.get());
    } else if (dynamic_cast<NumberValue*>(val.get())) {
        ret = ((NumberValue*) val.get())->get_value();
    } else {
        ret = val->get_value();
    }
    return ret;
}

bool Write::execute(VariableContext& scoped_vars) {
    for (auto& p : print_order) {
        std::cout << p->print(scoped_vars);
    }
    return true;
}