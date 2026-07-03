#include "components/literals/stringvalue.hpp"

#include <string>
#include <memory>

std::unique_ptr<Value> StringValue::clone() const {
    return std::make_unique<StringValue>(*this);
}

StringValue::StringValue() {
    std::string s = "";
    set_value(s);
}

StringValue::StringValue(std::string value) {
    set_value(std::ref(value));
}

StringValue::StringValue(const Value& val) {
    std::string v = val.get_value();
    set_value(v);
}

void StringValue::set_value(std::string& val) {
    this->len = val.length();
    Value::set_value(val);
}

void StringValue::set_value(Value& value) {
    std::string s = value.get_value();
    set_value(s);
}

int StringValue::get_len() const {
    return this->len;
}

StringValue StringValue::concat(Value& val1, Value& val2) {
    StringValue v1 = (StringValue) val1;
    StringValue v2 = (StringValue) val2;
    std::string ret = v1.get_value() + v2.get_value();
    return StringValue(ret);
}

StringValue StringValue::concat(Value& val1, std::string val2) {
    StringValue v1 = (StringValue) val1;
    std::string ret = v1.get_value() + val2;
    return StringValue(ret);
}

StringValue StringValue::concat(std::string val1, Value& val2) {
    StringValue v2 = (StringValue) val2;
    std::string ret = val1 + v2.get_value();
    return StringValue(ret);
}

StringValue StringValue::subst(Value& val, int start, int count, StringValue& string) {
    std::string s = val.get_value();
    std::string ret = s.replace(start, count, string.get_value());
    return StringValue(ret);
}

StringValue StringValue::subst(Value& val, int start, int count, std::string string) {
    std::string s = val.get_value();
    std::string ret = s.replace(start, count, string);
    return StringValue(ret);
}

StringValue operator+(StringValue& val1, StringValue& val2) {
    return StringValue::concat(val1, val2);
}

StringValue operator+(StringValue& val1, std::string val2) {
    return StringValue::concat(val1, val2);
}

StringValue operator+(std::string val1, StringValue& val2) {
    return StringValue::concat(val1, val2);
}

StringValue operator+(StringValue& val1, Value& val2) {
    return StringValue::concat(val1, val2);
}

StringValue operator+(Value& val1, StringValue& val2) {
    return StringValue::concat(val1, val2);
}

bool operator<(StringValue& val1, StringValue& val2) {
    std::string s1 = val1.get_value();
    std::string s2 = val2.get_value();
    return s1.compare(s2) < 0;
}

bool operator>(StringValue& val1, StringValue& val2) {
    std::string s1 = val1.get_value();
    std::string s2 = val2.get_value();
    return s1.compare(s2) > 0;
}

bool operator<=(StringValue& val1, StringValue& val2) {
    std::string s1 = val1.get_value();
    std::string s2 = val2.get_value();
    return s1.compare(s2) <= 0;
}

bool operator>=(StringValue& val1, StringValue& val2) {
    std::string s1 = val1.get_value();
    std::string s2 = val2.get_value();
    return s1.compare(s2) >= 0;
}

bool operator==(StringValue& val1, StringValue& val2) {
    std::string s1 = val1.get_value();
    std::string s2 = val2.get_value();
    return !s1.compare(s2);
}

bool operator!=(StringValue& val1, StringValue& val2) {
    std::string s1 = val1.get_value();
    std::string s2 = val2.get_value();
    return s1.compare(s2);
}

bool operator<(StringValue& val1, Value& val2) {
    StringValue v1 = val1;
    StringValue v2 = (StringValue) val2;
    return v1 < v2;
}

bool operator>(StringValue& val1, Value& val2) {
    StringValue v1 = val1;
    StringValue v2 = (StringValue) val2;
    return v1 > v2;
}

bool operator<=(StringValue& val1, Value& val2) {
    StringValue v1 = val1;
    StringValue v2 = (StringValue) val2;
    return v1 <= v2;
}

bool operator>=(StringValue& val1, Value& val2) {
    StringValue v1 = val1;
    StringValue v2 = (StringValue) val2;
    return v1 >= v2;
}

bool operator==(StringValue& val1, Value& val2) {
    StringValue v1 = val1;
    StringValue v2 = (StringValue) val2;
    return v1 == v2;
}

bool operator!=(StringValue& val1, Value& val2) {
    StringValue v1 = val1;
    StringValue v2 = (StringValue) val2;
    return v1 != v2;
}

bool operator<(Value& val1, StringValue& val2) {
    StringValue v1 = (StringValue) val1;
    StringValue v2 = val2;
    return v1 < v2;
}

bool operator>(Value& val1, StringValue& val2) {
    StringValue v1 = (StringValue) val1;
    StringValue v2 = val2;
    return v1 > v2;
}

bool operator<=(Value& val1, StringValue& val2) {
    StringValue v1 = (StringValue) val1;
    StringValue v2 = val2;
    return v1 <= v2;
}

bool operator>=(Value& val1, StringValue& val2) {
    StringValue v1 = (StringValue) val1;
    StringValue v2 = val2;
    return v1 >= v2;
}

bool operator==(Value& val1, StringValue& val2) {
    StringValue v1 = (StringValue) val1;
    StringValue v2 = val2;
    return v1 == v2;
}

bool operator!=(Value& val1, StringValue& val2) {
    StringValue v1 = (StringValue) val1;
    StringValue v2 = val2;
    return v1 != v2;
}

bool operator<(StringValue& val1, std::string val2) {
    StringValue v1 = val1;
    StringValue v2(val2);
    return v1 < v2;
}

bool operator>(StringValue& val1, std::string val2) {
    StringValue v1 = val1;
    StringValue v2(val2);
    return v1 > v2;
}

bool operator<=(StringValue& val1, std::string val2) {
    StringValue v1 = val1;
    StringValue v2(val2);
    return v1 <= v2;
}

bool operator>=(StringValue& val1, std::string val2) {
    StringValue v1 = val1;
    StringValue v2(val2);
    return v1 >= v2;
}

bool operator==(StringValue& val1, std::string val2) {
    StringValue v1 = val1;
    StringValue v2(val2);
    return v1 == v2;
}

bool operator!=(StringValue& val1, std::string val2) {
    StringValue v1 = val1;
    StringValue v2(val2);
    return v1 != v2;
}


bool operator<(std::string val1, StringValue& val2) {
    StringValue v1(val1);
    StringValue v2 = val2;
    return v1 < v2;
}

bool operator>(std::string val1, StringValue& val2) {
    StringValue v1(val1);
    StringValue v2 = val2;
    return v1 > v2;
}

bool operator<=(std::string val1, StringValue& val2) {
    StringValue v1(val1);
    StringValue v2 = val2;
    return v1 <= v2;
}

bool operator>=(std::string val1, StringValue& val2) {
    StringValue v1(val1);
    StringValue v2 = val2;
    return v1 >= v2;
}

bool operator==(std::string val1, StringValue& val2) {
    StringValue v1(val1);
    StringValue v2 = val2;
    return v1 == v2;
}

bool operator!=(std::string val1, StringValue& val2) {
    StringValue v1(val1);
    StringValue v2 = val2;
    return v1 != v2;
}

char& StringValue::operator[](int index) {
    if (index < 0 || index >= this->len) {
        throw std::runtime_error("index out of bounds (" + std::to_string(index) + ") for string '" + this->value + "'");
    }
    return this->value[index];
}

StringValue& StringValue::operator=(Value& val) {
    if (this != &val) {
        StringValue v = (StringValue) val;
        this->value = v.value;
        this->len = v.len;
    }
    return *this;
}

StringValue::operator NumberValue() const {
    std::string v = this->value;
    return NumberValue(v);
}