#include "components/literals/value.hpp"

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

std::string Value::get_value() const {
    return this->value;
}