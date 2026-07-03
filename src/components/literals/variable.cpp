#include "components/literals/variable.hpp"
#include "components/literals/value.hpp"

#include <string>
#include <vector>
#include <memory>
#include <exception>

Variable::Variable() {
    id = global_id;
    global_id++;
}

Variable::Variable(std::string& name) : Variable() {
    this->name = name;
}

Variable::Variable(std::string& name, Value& val) : Variable(name) {
    add_value(val);
}

Variable::Variable(Value& val) : Variable() {
    add_value(val);
}
    
int Variable::get_id() const {
    return id;
}

std::string Variable::get_name() const {
    return name;
}

void Variable::set_value(Value& val) {
    this->values.clear();
    add_value(val);
}

void Variable::set_value(Value&& val) {
    this->values.clear();
    add_value(val);
}

void Variable::set_value(std::unique_ptr<Value>& val) {
    this->values.clear();
    add_value(val);
}

Value* Variable::get_value() {
    return this->values[0].get();
}

Value* Variable::operator[](int idx) {
    std::size_t index = idx;
    if (index < 0 || index >= values.size()) {
        throw std::runtime_error("index out of bounds (" + std::to_string(index) + ") for variable '" + name + "' (id: " + std::to_string(id) + ")");
    }
    return values[index].get();
}

void Variable::add_value(Value& val) {
    values.push_back(val.clone());
}

void Variable::add_value(Value&& val) {
    add_value(static_cast<Value&>(val));
}

void Variable::add_value(std::unique_ptr<Value>& val) {
    values.push_back(std::move(val));
}

void Variable::remove_value(int idx) {
    std::size_t index = idx;
    if (index < 0 || index >= values.size()) {
        throw std::runtime_error("index out of bounds (" + std::to_string(index) + ") for variable '" + name + "' (id: " + std::to_string(id) + ")");
    }
    values.erase(values.begin() + index, values.begin() + index + 1);
}

int Variable::get_length() {
    return values.size();
}