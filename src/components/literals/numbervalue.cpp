#include "components/literals/numbervalue.hpp"

#include <string>
#include <algorithm>
#include <exception>
#include <cmath>
#include <memory>

std::unique_ptr<Value> NumberValue::clone() const {
    return std::make_unique<NumberValue>(*this);
}

NumberValue::NumberValue() {
    std::string s = "0";
    set_value(s);
}

NumberValue::NumberValue(std::string value) {
    set_value(std::ref(value));
}

NumberValue::NumberValue(const Value& val) {
    std::string v = val.get_value();
    set_value(v);
}

NumberValue::NumberValue(double value) {
    std::string v = std::to_string(value);
    set_value(v);
}

NumberValue::NumberValue(int value) {
    std::string v = std::to_string(value);
    set_value(v);
}

double NumberValue::get_value() const {
    return std::stod(value);
}

numbertype_e NumberValue::get_type() const {
    return type;
}

void NumberValue::set_value(std::string& val) {
    if (!is_number(val)) {
        throw std::runtime_error("value '" + val + "' is not a number");
    }

    if (val.contains(".")) {
        this->type = numbertype_e::Double;
    } else {
        this->type = numbertype_e::Integer;
    }

    Value::set_value(val);
}

void NumberValue::set_value(Value& val) {
    std::string s = val.get_value();
    set_value(s);
}

bool NumberValue::is_number(std::string& str) {
    if (str.empty()) {
        return false;
    }
    return std::all_of(str.begin(), str.end(), [](unsigned char c) { return std::isdigit(c) || c == '.' || c == '-'; });
}

NumberValue NumberValue::add(Value& val1, Value& val2) {
    NumberValue v1 = (NumberValue) val1;
    NumberValue v2 = (NumberValue) val2;
    return v1 + v2;
}

NumberValue NumberValue::sub(Value& val1, Value& val2) {
    NumberValue v1 = (NumberValue) val1;
    NumberValue v2 = (NumberValue) val2;
    return v1 - v2;
}

NumberValue NumberValue::mul(Value& val1, Value& val2) {
    NumberValue v1 = (NumberValue) val1;
    NumberValue v2 = (NumberValue) val2;
    return v1 * v2;
}

NumberValue NumberValue::div(Value& val1, Value& val2) {
    NumberValue v1 = (NumberValue) val1;
    NumberValue v2 = (NumberValue) val2;
    return v1 / v2;
}

NumberValue NumberValue::mod(Value& val1, Value& val2) {
    NumberValue v1 = (NumberValue) val1;
    NumberValue v2 = (NumberValue) val2;
    return v1 % v2;
}

NumberValue operator+(NumberValue& val1, NumberValue& val2) {
    double v1 = val1.get_value();
    double v2 = val2.get_value();

    if (val1.get_type() == numbertype_e::Integer && val2.get_type() == numbertype_e::Integer) {
        return NumberValue((int) v1 + (int) v2);
    }

    return NumberValue(v1 + v2);
}

NumberValue operator-(NumberValue& val1, NumberValue& val2) {
    double v1 = val1.get_value();
    double v2 = val2.get_value();

    if (val1.get_type() == numbertype_e::Integer && val2.get_type() == numbertype_e::Integer) {
        return NumberValue((int) v1 - (int) v2);
    }

    return NumberValue(v1 - v2);
}

NumberValue operator*(NumberValue& val1, NumberValue& val2) {
    double v1 = val1.get_value();
    double v2 = val2.get_value();

    if (val1.get_type() == numbertype_e::Integer && val2.get_type() == numbertype_e::Integer) {
        return NumberValue((int) v1 * (int) v2);
    }

    return NumberValue(v1 * v2);
}

NumberValue operator/(NumberValue& val1, NumberValue& val2) {
    double v1 = val1.get_value();
    double v2 = val2.get_value();

    if (v2 == 0) {
        throw std::runtime_error("0 division detected");
    }

    if (val1.get_type() == numbertype_e::Integer && val2.get_type() == numbertype_e::Integer) {
        return NumberValue((int) v1 / (int) v2);
    }

    return NumberValue(v1 / v2);
}

NumberValue operator%(NumberValue& val1, NumberValue& val2) {
    double v1 = (double) val1.get_value();
    double v2 = (double) val2.get_value();

    if (val1.get_type() == numbertype_e::Integer && val2.get_type() == numbertype_e::Integer) {
        return NumberValue((int) v1 % (int) v2);
    }

    return NumberValue(std::fmod(v1, v2));
}

NumberValue operator+(Value& val1, NumberValue& val2) {
    NumberValue v1 = (NumberValue) val1;
    NumberValue v2 = val2;
    return v1 + v2;
}

NumberValue operator-(Value& val1, NumberValue& val2) {
    NumberValue v1 = (NumberValue) val1;
    NumberValue v2 = val2;
    return v1 - v2;
}

NumberValue operator*(Value& val1, NumberValue& val2) {
    NumberValue v1 = (NumberValue) val1;
    NumberValue v2 = val2;
    return v1 * v2;
}

NumberValue operator/(Value& val1, NumberValue& val2) {
    NumberValue v1 = (NumberValue) val1;
    NumberValue v2 = val2;
    return v1 / v2;
}

NumberValue operator%(Value& val1, NumberValue& val2) {
    NumberValue v1 = (NumberValue) val1;
    NumberValue v2 = val2;
    return v1 % v2;
}

NumberValue operator+(NumberValue& val1, Value& val2) {
    NumberValue v1 = val1;
    NumberValue v2 = (NumberValue) val2;
    return v1 + v2;
}

NumberValue operator-(NumberValue& val1, Value& val2) {
    NumberValue v1 = val1;
    NumberValue v2 = (NumberValue) val2;
    return v1 - v2;
}

NumberValue operator*(NumberValue& val1, Value& val2) {
    NumberValue v1 = val1;
    NumberValue v2 = (NumberValue) val2;
    return v1 * v2;
}

NumberValue operator/(NumberValue& val1, Value& val2) {
    NumberValue v1 = val1;
    NumberValue v2 = (NumberValue) val2;
    return v1 / v2;
}

NumberValue operator%(NumberValue& val1, Value& val2) {
    NumberValue v1 = val1;
    NumberValue v2 = (NumberValue) val2;
    return v1 % v2;
}

NumberValue operator+(std::string val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = (NumberValue) val2;
    return v1 + v2;
}

NumberValue operator-(std::string val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = (NumberValue) val2;
    return v1 - v2;
}

NumberValue operator*(std::string val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = (NumberValue) val2;
    return v1 * v2;
}

NumberValue operator/(std::string val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = (NumberValue) val2;
    return v1 / v2;
}

NumberValue operator%(std::string val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = (NumberValue) val2;
    return v1 % v2;
}

NumberValue operator+(NumberValue& val1, std::string val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 + v2;
}

NumberValue operator-(NumberValue& val1, std::string val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 - v2;
}

NumberValue operator*(NumberValue& val1, std::string val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 * v2;
}

NumberValue operator/(NumberValue& val1, std::string val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 / v2;
}

NumberValue operator%(NumberValue& val1, std::string val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 % v2;
}

NumberValue operator+(int val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = (NumberValue) val2;
    return v1 + v2;
}

NumberValue operator-(int val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = (NumberValue) val2;
    return v1 - v2;
}

NumberValue operator*(int val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = (NumberValue) val2;
    return v1 * v2;
}

NumberValue operator/(int val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = (NumberValue) val2;
    return v1 / v2;
}

NumberValue operator%(int val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = (NumberValue) val2;
    return v1 % v2;
}

NumberValue operator+(NumberValue& val1, int val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 + v2;
}

NumberValue operator-(NumberValue& val1, int val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 - v2;
}

NumberValue operator*(NumberValue& val1, int val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 * v2;
}

NumberValue operator/(NumberValue& val1, int val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 / v2;
}

NumberValue operator%(NumberValue& val1, int val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 % v2;
}

NumberValue operator+(double val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = (NumberValue) val2;
    return v1 + v2;
}

NumberValue operator-(double val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = (NumberValue) val2;
    return v1 - v2;
}

NumberValue operator*(double val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = (NumberValue) val2;
    return v1 * v2;
}

NumberValue operator/(double val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = (NumberValue) val2;
    return v1 / v2;
}

NumberValue operator%(double val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = (NumberValue) val2;
    return v1 % v2;
}

NumberValue operator+(NumberValue& val1, double val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 + v2;
}

NumberValue operator-(NumberValue& val1, double val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 - v2;
}

NumberValue operator*(NumberValue& val1, double val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 * v2;
}

NumberValue operator/(NumberValue& val1, double val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 / v2;
}

NumberValue operator%(NumberValue& val1, double val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 % v2;
}

bool operator<(NumberValue& val1, NumberValue& val2) {
    double v1 = val1.get_value();
    double v2 = val2.get_value();

    if (val1.get_type() == numbertype_e::Integer && val2.get_type() == numbertype_e::Integer) {
        return ((int) v1) < ((int) v2);
    }

    return v1 < v2;
}

bool operator>(NumberValue& val1, NumberValue& val2) {
    double v1 = val1.get_value();
    double v2 = val2.get_value();

    if (val1.get_type() == numbertype_e::Integer && val2.get_type() == numbertype_e::Integer) {
        return ((int) v1) > ((int) v2);
    }

    return v1 > v2;
}

bool operator<=(NumberValue& val1, NumberValue& val2) {
    double v1 = val1.get_value();
    double v2 = val2.get_value();

    if (val1.get_type() == numbertype_e::Integer && val2.get_type() == numbertype_e::Integer) {
        return ((int) v1) <= ((int) v2);
    }

    return v1 <= v2;
}

bool operator>=(NumberValue& val1, NumberValue& val2) {
    double v1 = val1.get_value();
    double v2 = val2.get_value();

    if (val1.get_type() == numbertype_e::Integer && val2.get_type() == numbertype_e::Integer) {
        return ((int) v1) >= ((int) v2);
    }

    return v1 >= v2;
}

bool operator==(NumberValue& val1, NumberValue& val2) {
    double v1 = val1.get_value();
    double v2 = val2.get_value();

    if (val1.get_type() == numbertype_e::Integer && val2.get_type() == numbertype_e::Integer) {
        return ((int) v1) == ((int) v2);
    }

    return v1 == v2;
}

bool operator!=(NumberValue& val1, NumberValue& val2) {
    double v1 = val1.get_value();
    double v2 = val2.get_value();

    if (val1.get_type() == numbertype_e::Integer && val2.get_type() == numbertype_e::Integer) {
        return ((int) v1) != ((int) v2);
    }

    return v1 != v2;
}

bool operator<(Value& val1, NumberValue& val2) {
    NumberValue v1 = (NumberValue) val1;
    NumberValue v2 = val2;
    return v1 < v2;
}

bool operator>(Value& val1, NumberValue& val2) {
    NumberValue v1 = (NumberValue) val1;
    NumberValue v2 = val2;
    return v1 > v2;
}

bool operator<=(Value& val1, NumberValue& val2) {
    NumberValue v1 = (NumberValue) val1;
    NumberValue v2 = val2;
    return v1 <= v2;
}

bool operator>=(Value& val1, NumberValue& val2) {
    NumberValue v1 = (NumberValue) val1;
    NumberValue v2 = val2;
    return v1 >= v2;
}

bool operator==(Value& val1, NumberValue& val2) {
    NumberValue v1 = (NumberValue) val1;
    NumberValue v2 = val2;
    return v1 == v2;
}

bool operator!=(Value& val1, NumberValue& val2) {
    NumberValue v1 = (NumberValue) val1;
    NumberValue v2 = val2;
    return v1 != v2;
}


bool operator<(NumberValue& val1, Value& val2) {
    NumberValue v1 = val1;
    NumberValue v2 = (NumberValue) val2;
    return v1 < v2;
}

bool operator>(NumberValue& val1, Value& val2) {
    NumberValue v1 = val1;
    NumberValue v2 = (NumberValue) val2;
    return v1 > v2;
}

bool operator<=(NumberValue& val1, Value& val2) {
    NumberValue v1 = val1;
    NumberValue v2 = (NumberValue) val2;
    return v1 <= v2;
}

bool operator>=(NumberValue& val1, Value& val2) {
    NumberValue v1 = val1;
    NumberValue v2 = (NumberValue) val2;
    return v1 >= v2;
}

bool operator==(NumberValue& val1, Value& val2) {
    NumberValue v1 = val1;
    NumberValue v2 = (NumberValue) val2;
    return v1 == v2;
}

bool operator!=(NumberValue& val1, Value& val2) {
    NumberValue v1 = val1;
    NumberValue v2 = (NumberValue) val2;
    return v1 != v2;
}

bool operator<(NumberValue& val1, std::string val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 < v2;
}

bool operator>(NumberValue& val1, std::string val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 > v2;
}

bool operator<=(NumberValue& val1, std::string val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 <= v2;
}

bool operator>=(NumberValue& val1, std::string val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 >= v2;
}

bool operator==(NumberValue& val1, std::string val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 == v2;
}

bool operator!=(NumberValue& val1, std::string val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 != v2;
}

bool operator<(std::string val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = val2;
    return v1 < v2;
}

bool operator>(std::string val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = val2;
    return v1 > v2;
}

bool operator<=(std::string val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = val2;
    return v1 <= v2;
}

bool operator>=(std::string val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = val2;
    return v1 >= v2;
}

bool operator==(std::string val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = val2;
    return v1 == v2;
}

bool operator!=(std::string val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = val2;
    return v1 != v2;
}

bool operator<(NumberValue& val1, int val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 < v2;
}

bool operator>(NumberValue& val1, int val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 > v2;
}

bool operator<=(NumberValue& val1, int val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 <= v2;
}

bool operator>=(NumberValue& val1, int val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 >= v2;
}

bool operator==(NumberValue& val1, int val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 == v2;
}

bool operator!=(NumberValue& val1, int val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 != v2;
}

bool operator<(int val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = val2;
    return v1 < v2;
}

bool operator>(int val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = val2;
    return v1 > v2;
}

bool operator<=(int val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = val2;
    return v1 <= v2;
}

bool operator>=(int val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = val2;
    return v1 >= v2;
}

bool operator==(int val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = val2;
    return v1 == v2;
}

bool operator!=(int val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = val2;
    return v1 != v2;
}

bool operator<(NumberValue& val1, double val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 < v2;
}

bool operator>(NumberValue& val1, double val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 > v2;
}

bool operator<=(NumberValue& val1, double val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 <= v2;
}

bool operator>=(NumberValue& val1, double val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 >= v2;
}

bool operator==(NumberValue& val1, double val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 == v2;
}

bool operator!=(NumberValue& val1, double val2) {
    NumberValue v1 = val1;
    NumberValue v2(val2);
    return v1 != v2;
}


bool operator<(double val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = val2;
    return v1 < v2;
}

bool operator>(double val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = val2;
    return v1 > v2;
}

bool operator<=(double val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = val2;
    return v1 <= v2;
}

bool operator>=(double val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = val2;
    return v1 >= v2;
}

bool operator==(double val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = val2;
    return v1 == v2;
}

bool operator!=(double val1, NumberValue& val2) {
    NumberValue v1(val1);
    NumberValue v2 = val2;
    return v1 != v2;
}

NumberValue& NumberValue::operator=(Value& val) {
    if (this != &val) {
        NumberValue v = (NumberValue) val;
        this->value = v.value;
    }
    return *this;
}

NumberValue::operator StringValue() const {
    std::string s;
    if (this->get_type() == numbertype_e::Double) {
        s = std::to_string(this->get_value());
    } else {
        s = std::to_string((int) this->get_value());
    }
    return StringValue(s);
}