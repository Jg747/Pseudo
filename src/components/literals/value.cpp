#include "components/literals/value.hpp"
#include "components/literals/numbervalue.hpp"
#include "components/literals/stringvalue.hpp"
#include "components/literals/arrayvalue.hpp"
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
    if (dynamic_cast<NumberValue*>(&val1) && !dynamic_cast<ArrayValue*>(&val2)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 + v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            StringValue& v2 = static_cast<StringValue&>(val2);
            return ((StringValue) v1 + v2).clone();
        }
    } else if (dynamic_cast<StringValue*>(&val1) && !dynamic_cast<ArrayValue*>(&val2)) {
        StringValue& v1 = static_cast<StringValue&>(val1);
        StringValue v2 = StringValue(val2);
        return (v1 + v2).clone();
    } else if (dynamic_cast<ArrayValue*>(&val1)) {
        ArrayValue& v1 = static_cast<ArrayValue&>(val1);
        return (v1 + val2).clone();
    } else if (dynamic_cast<ArrayValue*>(&val2)) {
        ArrayValue& v2 = static_cast<ArrayValue&>(val2);
        return (val1 + v2).clone();
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
        NumberValue v1(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 - v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return (v1 - v2).clone();
        }
    } else if (dynamic_cast<ArrayValue*>(&val1)) {
        ArrayValue& arr = (ArrayValue&) val1;
        return (arr - val2).clone();
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
        NumberValue v1(val1);
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

std::unique_ptr<Value> pow(Value& val1, Value& val2) {
    if (dynamic_cast<NumberValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return NumberValue::pow(v1, v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return NumberValue::pow(v1, v2).clone();
        }
    } else if (dynamic_cast<StringValue*>(&val1)) {
        NumberValue v1(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return NumberValue::pow(v1, v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return NumberValue::pow(v1, v2).clone();
        }
    }
    throw std::runtime_error("Invalid cast Value provided (operator '**')");
}

std::unique_ptr<Value> int_pow(Value& val1, Value& val2) {
    if (dynamic_cast<NumberValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return NumberValue::int_pow(v1, v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return NumberValue::int_pow(v1, v2).clone();
        }
    } else if (dynamic_cast<StringValue*>(&val1)) {
        NumberValue v1(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return NumberValue::int_pow(v1, v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return NumberValue::int_pow(v1, v2).clone();
        }
    }
    throw std::runtime_error("Invalid cast Value provided (operator '**')");
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
        NumberValue v1(val1);
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

std::unique_ptr<Value> int_div(Value& val1, Value& val2) {
    if (dynamic_cast<NumberValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return NumberValue::int_div(v1, v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return NumberValue::int_div(v1, v2).clone();
        }
    } else if (dynamic_cast<StringValue*>(&val1)) {
        NumberValue v1(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return NumberValue::int_div(v1, v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return NumberValue::int_div(v1, v2).clone();
        }
    }
    throw std::runtime_error("Invalid cast Value provided (operator '//')");
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
        NumberValue v1(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return (v1 % v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return (v1 % v2).clone();
        }
    }
    throw std::runtime_error("Invalid cast Value provided (operator '%%')");
}

std::unique_ptr<Value> int_mod(Value& val1, Value& val2) {
    if (dynamic_cast<NumberValue*>(&val1)) {
        NumberValue& v1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return NumberValue::int_mod(v1, v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return NumberValue::int_mod(v1, v2).clone();
        }
    } else if (dynamic_cast<StringValue*>(&val1)) {
        NumberValue v1(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& v2 = static_cast<NumberValue&>(val2);
            return NumberValue::int_mod(v1, v2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            NumberValue v2(val2);
            return NumberValue::int_mod(v1, v2).clone();
        }
    }
    throw std::runtime_error("Invalid cast Value provided (operator '%')");
}

std::unique_ptr<Value> operator<(Value& val1, Value& val2) {
    if (dynamic_cast<NumberValue*>(&val1)) {
        NumberValue& n1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& n2 = static_cast<NumberValue&>(val2);
            return (n1 < n2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            try {
                NumberValue n2(val2);
                return (n1 < n2).clone();
            } catch (std::runtime_error& e) {}
            StringValue s1(n1);
            StringValue& s2 = (StringValue&) val2;
            return (s1 < s2).clone();
        }
    } else if (dynamic_cast<StringValue*>(&val1)) {
        StringValue& s1 = static_cast<StringValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            try {
                NumberValue n1(s1);
                NumberValue n2 = NumberValue((NumberValue&) val2);
                return (n1 < n2).clone();
            } catch (std::runtime_error& e) {
                StringValue s2((NumberValue&) val2);
                return (s1 < s2).clone();
            }
        } else if (dynamic_cast<StringValue*>(&val2)) {
            StringValue& s2 = (StringValue&) val2;
            return (s1 < s2).clone();
        }
    }
    throw std::runtime_error("Invalid cast Value provided (operator '<')");
}

std::unique_ptr<Value> operator>(Value& val1, Value& val2) {
    if (dynamic_cast<NumberValue*>(&val1)) {
        NumberValue& n1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& n2 = static_cast<NumberValue&>(val2);
            return (n1 > n2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            try {
                NumberValue n2(val2);
                return (n1 > n2).clone();
            } catch (std::runtime_error& e) {}
            StringValue s1(n1);
            StringValue& s2 = (StringValue&) val2;
            return (s1 > s2).clone();
        }
    } else if (dynamic_cast<StringValue*>(&val1)) {
        StringValue& s1 = static_cast<StringValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            try {
                NumberValue n1(s1);
                NumberValue n2 = NumberValue((NumberValue&) val2);
                return (n1 > n2).clone();
            } catch (std::runtime_error& e) {
                StringValue s2((NumberValue&) val2);
                return (s1 > s2).clone();
            }
        } else if (dynamic_cast<StringValue*>(&val2)) {
            StringValue& s2 = (StringValue&) val2;
            return (s1 > s2).clone();
        }
    }
    throw std::runtime_error("Invalid cast Value provided (operator '>')");
}

std::unique_ptr<Value> operator<=(Value& val1, Value& val2) {
    if (dynamic_cast<NumberValue*>(&val1)) {
        NumberValue& n1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& n2 = static_cast<NumberValue&>(val2);
            return (n1 <= n2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            try {
                NumberValue n2(val2);
                return (n1 <= n2).clone();
            } catch (std::runtime_error& e) {}
            StringValue s1(n1);
            StringValue& s2 = (StringValue&) val2;
            return (s1 <= s2).clone();
        }
    } else if (dynamic_cast<StringValue*>(&val1)) {
        StringValue& s1 = static_cast<StringValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            try {
                NumberValue n1(s1);
                NumberValue n2 = NumberValue((NumberValue&) val2);
                return (n1 <= n2).clone();
            } catch (std::runtime_error& e) {
                StringValue s2((NumberValue&) val2);
                return (s1 <= s2).clone();
            }
        } else if (dynamic_cast<StringValue*>(&val2)) {
            StringValue& s2 = (StringValue&) val2;
            return (s1 <= s2).clone();
        }
    }
    throw std::runtime_error("Invalid cast Value provided (operator '<=')");
}

std::unique_ptr<Value> operator>=(Value& val1, Value& val2) {
    if (dynamic_cast<NumberValue*>(&val1)) {
        NumberValue& n1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& n2 = static_cast<NumberValue&>(val2);
            return (n1 >= n2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            try {
                NumberValue n2(val2);
                return (n1 >= n2).clone();
            } catch (std::runtime_error& e) {}
            StringValue s1(n1);
            StringValue& s2 = (StringValue&) val2;
            return (s1 >= s2).clone();
        }
    } else if (dynamic_cast<StringValue*>(&val1)) {
        StringValue& s1 = static_cast<StringValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            try {
                NumberValue n1(s1);
                NumberValue n2 = NumberValue((NumberValue&) val2);
                return (n1 >= n2).clone();
            } catch (std::runtime_error& e) {
                StringValue s2((NumberValue&) val2);
                return (s1 >= s2).clone();
            }
        } else if (dynamic_cast<StringValue*>(&val2)) {
            StringValue& s2 = (StringValue&) val2;
            return (s1 >= s2).clone();
        }
    }
    throw std::runtime_error("Invalid cast Value provided (operator '>=')");
}

std::unique_ptr<Value> operator==(Value& val1, Value& val2) {
    if (dynamic_cast<NumberValue*>(&val1)) {
        NumberValue& n1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& n2 = static_cast<NumberValue&>(val2);
            return (n1 == n2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            try {
                NumberValue n2(val2);
                return (n1 == n2).clone();
            } catch (std::runtime_error& e) {}
            StringValue s1(n1);
            StringValue& s2 = (StringValue&) val2;
            return (s1 == s2).clone();
        }
    } else if (dynamic_cast<StringValue*>(&val1)) {
        StringValue& s1 = static_cast<StringValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            try {
                NumberValue n1(s1);
                NumberValue n2 = NumberValue((NumberValue&) val2);
                return (n1 == n2).clone();
            } catch (std::runtime_error& e) {
                StringValue s2((NumberValue&) val2);
                return (s1 == s2).clone();
            }
        } else if (dynamic_cast<StringValue*>(&val2)) {
            StringValue& s2 = (StringValue&) val2;
            return (s1 == s2).clone();
        }
    }
    throw std::runtime_error("Invalid cast Value provided (operator '==')");
}

std::unique_ptr<Value> operator!=(Value& val1, Value& val2) {
    if (dynamic_cast<NumberValue*>(&val1)) {
        NumberValue& n1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& n2 = static_cast<NumberValue&>(val2);
            return (n1 != n2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            try {
                NumberValue n2(val2);
                return (n1 != n2).clone();
            } catch (std::runtime_error& e) {}
            StringValue s1(n1);
            StringValue& s2 = (StringValue&) val2;
            return (s1 != s2).clone();
        }
    } else if (dynamic_cast<StringValue*>(&val1)) {
        StringValue& s1 = static_cast<StringValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            try {
                NumberValue n1(s1);
                NumberValue n2 = NumberValue((NumberValue&) val2);
                return (n1 != n2).clone();
            } catch (std::runtime_error& e) {
                StringValue s2((NumberValue&) val2);
                return (s1 != s2).clone();
            }
        } else if (dynamic_cast<StringValue*>(&val2)) {
            StringValue& s2 = (StringValue&) val2;
            return (s1 != s2).clone();
        }
    }
    throw std::runtime_error("Invalid cast Value provided (operator '!=')");
}

std::unique_ptr<Value> operator&&(Value& val1, Value& val2) {
    if (dynamic_cast<NumberValue*>(&val1)) {
        NumberValue& n1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& n2 = static_cast<NumberValue&>(val2);
            return (n1 && n2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            try {
                NumberValue n2(val2);
                return (n1 && n2).clone();
            } catch (std::runtime_error& e) {}
            StringValue s1(n1);
            StringValue& s2 = (StringValue&) val2;
            return (s1 && s2).clone();
        }
    } else if (dynamic_cast<StringValue*>(&val1)) {
        StringValue& s1 = static_cast<StringValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            try {
                NumberValue n1(s1);
                NumberValue n2 = NumberValue((NumberValue&) val2);
                return (n1 && n2).clone();
            } catch (std::runtime_error& e) {
                StringValue s2((NumberValue&) val2);
                return (s1 && s2).clone();
            }
        } else if (dynamic_cast<StringValue*>(&val2)) {
            StringValue& s2 = (StringValue&) val2;
            return (s1 && s2).clone();
        }
    }
    throw std::runtime_error("Invalid cast Value provided (operator '&&')");
}

std::unique_ptr<Value> operator||(Value& val1, Value& val2) {
    if (dynamic_cast<NumberValue*>(&val1)) {
        NumberValue& n1 = static_cast<NumberValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            NumberValue& n2 = static_cast<NumberValue&>(val2);
            return (n1 || n2).clone();
        } else if (dynamic_cast<StringValue*>(&val2)) {
            try {
                NumberValue n2(val2);
                return (n1 || n2).clone();
            } catch (std::runtime_error& e) {}
            StringValue s1(n1);
            StringValue& s2 = (StringValue&) val2;
            return (s1 || s2).clone();
        }
    } else if (dynamic_cast<StringValue*>(&val1)) {
        StringValue& s1 = static_cast<StringValue&>(val1);
        if (dynamic_cast<NumberValue*>(&val2)) {
            try {
                NumberValue n1(s1);
                NumberValue n2 = NumberValue((NumberValue&) val2);
                return (n1 || n2).clone();
            } catch (std::runtime_error& e) {
                StringValue s2((NumberValue&) val2);
                return (s1 || s2).clone();
            }
        } else if (dynamic_cast<StringValue*>(&val2)) {
            StringValue& s2 = (StringValue&) val2;
            return (s1 || s2).clone();
        }
    }
    throw std::runtime_error("Invalid cast Value provided (operator '||')");
}
