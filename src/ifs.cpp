#include "ifs.hpp"

If::If() {}

If::If(enum if_type_e type) {
    this->type = type;
}

void If::set_if_type(enum if_type_e type) {
    this->type = type;
}

enum if_type_e If::get_if_type() {
    return type;
}

void If::add_condition(struct condition_t cond) {
    this->conditions.push_back(cond);
}

std::vector<struct condition_t>& If::get_condition_array() {
    return this->conditions;
}