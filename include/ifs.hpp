/*
Contains object to represent ifs
- if
- if else
- if else if ... else
- switch case
*/

#pragma once

#include <string>
#include <vector>

enum if_type_e {
    IF,
    ELSE_IF,
    SWITCH
};

struct condition_t {
    std::string condition;  // condition
    /*
    case IF: 
        jump_false = index of the first row not in if
        endif = last index of row in if
    case ELSE_IF
        jump_false = program will check other conditions first, if all false it will jump to last_condition::endif + 1 
        endif = last index of row in if
    case SWITCH
        jump_false = program will check other conditions first, if all false it will jump to (last_condition - 1)::endif + 1 (if DEFAULT) or last_condition::endif + 1 (if no DEFAULT)
        endif = last index of row in if
    */
    std::size_t jump_false; // index of where to jump if condition is false
    std::size_t endif;      // max_index of code into the if
};

class If {
    private:
        enum if_type_e type;
        std::vector<struct condition_t> conditions;
    
    public:
        If();
        If(enum if_type_e type);

        void set_if_type(enum if_type_e type);
        enum if_type_e get_if_type();
        void add_condition(struct condition_t cond);
        std::vector<struct condition_t>& get_condition_array();
};