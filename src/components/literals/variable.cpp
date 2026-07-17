#include "components/literals/variable.hpp"
#include "components/literals/value.hpp"
#include "components/literals/arrayvalue.hpp"
#include "lang.hpp"

#include <string>
#include <vector>
#include <memory>
#include <exception>
#include <regex>

Variable::Variable() {
    id = global_id;
    global_id++;
}

Variable::Variable(std::string& name) : Variable() {
    this->name = name;
    is_arr = false;
}

Variable::Variable(std::string&& name) : Variable() {
    this->name = name;
    is_arr = false;
}

Variable::Variable(std::string& name, Value& val) : Variable(name) {
    value = std::move(val.clone());
    is_arr = (dynamic_cast<ArrayValue*>(&val) != NULL);
}

Variable::Variable(std::string& name, Value&& val) : Variable(name) {
    value = std::move(val.clone());
    is_arr = (dynamic_cast<ArrayValue*>(&val) != NULL);
}

Variable::Variable(std::string&& name, Value& val) : Variable(name) {
    value = std::move(val.clone());
    is_arr = (dynamic_cast<ArrayValue*>(&val) != NULL);
}

Variable::Variable(std::string&& name, Value&& val) : Variable(name) {
    value = std::move(val.clone());
    is_arr = (dynamic_cast<ArrayValue*>(&val) != NULL);
}

Variable::Variable(Value& val) : Variable() {
    value = std::move(val.clone());
    is_arr = (dynamic_cast<ArrayValue*>(&val) != NULL);
}

Variable::Variable(Value&& val) : Variable() {
    value = std::move(val.clone());
    is_arr = (dynamic_cast<ArrayValue*>(&val) != NULL);
}
    
int Variable::get_id() const {
    return id;
}

std::string Variable::get_name() const {
    return name;
}

bool Variable::is_array() const {
    return is_arr;
}

void Variable::set_value(std::unique_ptr<Value> val) {
    value = std::move(val);
}

void Variable::set_value(std::shared_ptr<Value> val) {
    value = std::move(val);
}

void Variable::set_value(Value& val) {
    value = val.clone();
}

void Variable::set_value(Value&& val) {
    value = val.clone();
}

std::shared_ptr<Value> Variable::get_value() {
    return value;
}

std::shared_ptr<ArrayValue> Variable::get_array_value() {
    if (!is_arr) {
        return nullptr;
    }
    return std::dynamic_pointer_cast<ArrayValue>(value);
}

bool Variable::is_name_correct(std::string name) {
    std::regex reg(ALLOWED_VARSFUNCS_CHARS);
    return std::regex_match(name, reg);
}