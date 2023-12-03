#include "variables.hpp"
#include "exceptions.hpp"

#include <string>
#include <vector>

Variable::Variable() {
    var_type = UNDEFINED;
    isarray = false;
}

Variable::Variable(std::string var_name) {
    var_type = UNDEFINED;
    isarray = false;
    this->var_name = var_name;
}

Variable::Variable(std::string var_name, std::string var_value) {
    this->var_name = var_name;
    set_var_value(var_value);
}

enum variable_type_e Variable::get_var_type() {
    return this->var_type;
}

std::string Variable::get_var_name() {
    return this->var_name;
}

std::string Variable::get_var_value(std::size_t idx) {
    if (idx < 0 || idx >= var_value.size()) {
        throw VariableException(1);
    }

    if (idx != 0 && !isarray) {
        throw VariableException(2);
    }

    return var_value[idx];
}

bool Variable::is_array() {
    return isarray;
}

void Variable::set_var_type(enum variable_type_e type) {
    this->var_type = type;
}

void Variable::set_var_name(std::string var_name) {
    this->var_name = var_name;
}

void Variable::set_var_value(std::string var_value) {
    if (var_value.size() == 0) {
        this->var_value.push_back(var_value);
    } else {
        this->var_value[0] = var_value;
    }
}

void Variable::set_var_value(std::string var_value, std::size_t idx) {
    if (idx < 0 || idx >= var_value.size()) {
        throw VariableException(1);
    }

    if (idx != 0 && !isarray) {
        throw VariableException(2);
    }

    this->var_value[idx] = var_value;
}

void Variable::add_var_value(std::string var_value) {
    if (!isarray) {
        throw VariableException(2);
    }
    this->var_value.push_back(var_value);
}

void Variable::set_array_mode(bool array) {
    isarray = array;
}