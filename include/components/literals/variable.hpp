#ifndef __VARIABLE_HPP__
#define __VARIABLE_HPP__

#include "value.hpp"

#include <string>
#include <memory>
#include <vector>

class Variable {
private:
    std::string name;
    
    long id;
    static inline long global_id = 0;
    
    std::vector<std::unique_ptr<Value>> values;
public:
    Variable();
    Variable(std::string& name);
    Variable(std::string& name, Value& val);
    Variable(Value& val);
    
    int get_id() const;
    std::string get_name() const;

    void set_value(std::unique_ptr<Value>& val);
    void set_value(Value& val);
    void set_value(Value&& val);
    Value* get_value();

    Value* operator[](int idx);
    void add_value(Value& val);
    void add_value(Value&& val);
    void add_value(std::unique_ptr<Value>& val);
    void remove_value(int idx);
    int get_length();
};

#endif // __VARIABLE_HPP__