#pragma once

#include <string>
#include <vector>

enum variable_type_e {
    UNDEFINED,
    INTEGER,
    DOUBLE,
    BOOLEAN,
    STRING
};

class Variable {
    private:
        enum variable_type_e var_type;
        bool isarray;
        std::string var_name;
        std::vector<std::string> var_value;
    
    public:
        Variable();
        Variable(std::string var_name);
        Variable(std::string var_name, std::string var_value);

        enum variable_type_e get_var_type();
        std::string get_var_name();
        std::string get_var_value(std::size_t idx);
        bool is_array();

        void set_var_type(enum variable_type_e type);
        void set_var_name(std::string var_name);
        void set_var_value(std::string var_value);
        void add_var_value(std::string var_value);
        void set_var_value(std::string var_value, std::size_t idx);
        void set_array_mode(bool array);
};