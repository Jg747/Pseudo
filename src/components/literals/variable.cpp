#include "components/literals/variable.hpp"
#include "components/literals/value.hpp"
#include "components/literals/arrayvalue.hpp"
#include "components/literals/numbervalue.hpp"
#include "components/literals/specialvalue.hpp"
#include "interpreter/analyzers/syntaxanalyzer.hpp"
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

    set_value(NullValue());
}

Variable::Variable(std::string&& name) : Variable() {
    this->name = name;
    is_arr = false;

    set_value(NullValue());
}

Variable::Variable(std::string& name, Value& val) : Variable(name) {
    set_value(val);
}

Variable::Variable(std::string& name, Value&& val) : Variable(name) {
    set_value(val);
}

Variable::Variable(std::string&& name, Value& val) : Variable(name) {
    set_value(val);
}

Variable::Variable(std::string&& name, Value&& val) : Variable(name) {
    set_value(val);
}

Variable::Variable(Value& val) : Variable() {
    set_value(val);
}

Variable::Variable(Value&& val) : Variable() {
    set_value(val);
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
    is_arr = (dynamic_cast<ArrayValue*>(val.get()) != NULL);
    value->set_variable(this);
}

void Variable::set_value(std::shared_ptr<Value> val) {
    value = std::move(val);
    is_arr = (dynamic_cast<ArrayValue*>(val.get()) != NULL);
    value->set_variable(this);
}

void Variable::set_value(Value& val) {
    value = val.clone();
    is_arr = (dynamic_cast<ArrayValue*>(&val) != NULL);
    value->set_variable(this);
}

void Variable::set_value(Value&& val) {
    value = val.clone();
    is_arr = (dynamic_cast<ArrayValue*>(&val) != NULL);
    value->set_variable(this);
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
    std::regex reg(ALLOWED_VARS_CHARS);
    return std::regex_match(name, reg) && !SyntaxAnalyzer::is_keyword(name);
}