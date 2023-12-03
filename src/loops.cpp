#include "loops.hpp"

#include <string>

Loop::Loop() {}

Loop::Loop(enum loop_type_e type) {
    this->type = type;
}

void Loop::set_condition(std::string condition) {
    this->condition = condition;
}

void Loop::set_start_idx(std::size_t start_idx) {
    this->start_idx = start_idx;
}

void Loop::set_end_idx(std::size_t end_idx) {
    this->end_idx = end_idx;
}

void Loop::set_jump_false(std::size_t jump_false) {
    this->jump_false = jump_false;
}

std::string Loop::get_condition() {
    return condition;
}

std::size_t Loop::get_start_idx() {
    return start_idx;
}

std::size_t Loop::get_end_idx() {
    return end_idx;
}

std::size_t Loop::get_jump_false() {
    return jump_false;
}



For::For() : Loop(WHILE) {}
        
void For::set_initializer(std::string init) {
    initializer = init;
}

void For::set_update(std::string update) {
    this->update = update;
}

std::string For::get_initializer() {
    return initializer;
}

std::string For::get_update() {
    return update;
}