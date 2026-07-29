#include "components/literals/value.hpp"
#include "components/literals/numbervalue.hpp"
#include "components/literals/stringvalue.hpp"
#include "components/literals/variable.hpp"

#include <string>

Value::Value() {}

Value::Value(const Value& val) {
    this->value = val.value;
}

Value::Value(std::string value) {
    this->value = value;
}

Value::Value(int value) {
    this->value = std::to_string(value);
}

Value::Value(float value) {
    this->value = std::to_string(value);
}

Value::Value(double value) {
    this->value = std::to_string(value);
}

Value::~Value() {}

void Value::set_value(std::string& val) {
    this->value = val;
}

void Value::set_value(Value& val) {
    this->value = val.value;
}

void Value::set_variable(Variable* var) {
    this->assign.var = var;
    assignment_var = true;
}

void Value::set_assign_array(ArrayValue* arr) {
    this->assign.arr = arr;
    assignment_var = false;
}

bool Value::assigment_type() const {
    return assignment_var;
}

ArrayValue* Value::get_assign_array() const {
    return this->assign.arr;
}

Variable* Value::get_variable() const {
    return this->assign.var;
}

std::string Value::get_value() const {
    return this->value;
}

std::unique_ptr<Value> operator+(Value& val1, Value& val2) {
    if (dynamic_cast<NumberValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 + v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            StringValue& v2 = static_cast<StringValue&>(val2);
            return ((StringValue&) v1 + v2).clone();
        }
    } else if (dynamic_cast<StringValue*>(&val1)) {
        StringValue& v1 = static_cast<StringValue&>(val1);
        StringValue& v2 = static_cast<StringValue&>(val2);
        return (v1 + v2).clone();
    }
    throw std::runtime_error("Invalid cast Value provided (operator '+')");
}

std::unique_ptr<Value> operator-(Value& val1, Value& val2) {
    if (dynamic_cast<NumberValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 - v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return (v1 - v2).clone();
        }
    } else if (dynamic_cast<StringValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 - v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return (v1 - v2).clone();
        }
    }
    throw std::runtime_error("Invalid cast Value provided (operator '-')");
}

std::unique_ptr<Value> operator*(Value& val1, Value& val2) {
    if (dynamic_cast<NumberValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 * v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return (v1 * v2).clone();
        }
    } else if (dynamic_cast<StringValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 * v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return (v1 * v2).clone();
        }
    }
    throw std::runtime_error("Invalid cast Value provided (operator '*')");
}

std::unique_ptr<Value> operator/(Value& val1, Value& val2) {
    if (dynamic_cast<NumberValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 / v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return (v1 / v2).clone();
        }
    } else if (dynamic_cast<StringValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 / v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return (v1 / v2).clone();
        }
    }
    throw std::runtime_error("Invalid cast Value provided (operator '/')");
}

std::unique_ptr<Value> operator%(Value& val1, Value& val2) {
    if (dynamic_cast<NumberValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 % v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return (v1 % v2).clone();
        }
    } else if (dynamic_cast<StringValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 % v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return (v1 % v2).clone();
        }
    }
    throw std::runtime_error("Invalid cast Value provided (operator '%')");
}

std::unique_ptr<Value> operator<(Value& val1, Value& val2) {
    if (dynamic_cast<NumberValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 < v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return (v1 < v2).clone();
        }
    } else if (dynamic_cast<StringValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 < v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return (v1 < v2).clone();
        }
    }
    throw std::runtime_error("Invalid cast Value provided (operator '<')");
}

std::unique_ptr<Value> operator>(Value& val1, Value& val2) {
    if (dynamic_cast<NumberValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 > v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return (v1 > v2).clone();
        }
    } else if (dynamic_cast<StringValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 > v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return (v1 > v2).clone();
        }
    }
    throw std::runtime_error("Invalid cast Value provided (operator '>')");
}

std::unique_ptr<Value> operator<=(Value& val1, Value& val2) {
    if (dynamic_cast<NumberValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 <= v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return (v1 <= v2).clone();
        }
    } else if (dynamic_cast<StringValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 <= v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return (v1 <= v2).clone();
        }
    }
    throw std::runtime_error("Invalid cast Value provided (operator '<=')");
}

std::unique_ptr<Value> operator>=(Value& val1, Value& val2) {
    if (dynamic_cast<NumberValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 >= v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return (v1 >= v2).clone();
        }
    } else if (dynamic_cast<StringValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 >= v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return (v1 >= v2).clone();
        }
    }
    throw std::runtime_error("Invalid cast Value provided (operator '>=')");
}

std::unique_ptr<Value> operator==(Value& val1, Value& val2) {
    if (dynamic_cast<NumberValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 == v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return (v1 == v2).clone();
        }
    } else if (dynamic_cast<StringValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 == v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return (v1 == v2).clone();
        }
    }
    throw std::runtime_error("Invalid cast Value provided (operator '==')");
}

std::unique_ptr<Value> operator!=(Value& val1, Value& val2) {
    if (dynamic_cast<NumberValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 != v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return (v1 != v2).clone();
        }
    } else if (dynamic_cast<StringValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 != v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return (v1 != v2).clone();
        }
    }
    throw std::runtime_error("Invalid cast Value provided (operator '!=')");
}

std::unique_ptr<Value> operator&&(Value& val1, Value& val2) {
    if (dynamic_cast<NumberValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 && v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return (v1 && v2).clone();
        }
    } else if (dynamic_cast<StringValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 && v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return (v1 && v2).clone();
        }
    }
    throw std::runtime_error("Invalid cast Value provided (operator '&&')");
}

std::unique_ptr<Value> operator||(Value& val1, Value& val2) {
    if (dynamic_cast<NumberValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 || v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return (v1 || v2).clone();
        }
    } else if (dynamic_cast<StringValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 || v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return (v1 || v2).clone();
        }
    }
    throw std::runtime_error("Invalid cast Value provided (operator '||')");
}
