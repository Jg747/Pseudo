#ifndef __VARIABLE_HPP__
#define __VARIABLE_HPP__

#include "value.hpp"
#include "arrayvalue.hpp"

#include <string>
#include <memory>

class Variable {
private:
    long id;
    static inline long global_id = 0;
    
    std::string name;
    std::shared_ptr<Value> value;
    bool is_arr;

public:
    static bool is_name_correct(std::string name);

    Variable();
    Variable(std::string& name);
    Variable(std::string&& name);
    Variable(std::string& name, Value& val);
    Variable(std::string& name, Value&& val);
    Variable(std::string&& name, Value& val);
    Variable(std::string&& name, Value&& val);
    Variable(Value& val);
    Variable(Value&& val);
    
    int get_id() const;
    std::string get_name() const;
    bool is_array() const;

    void set_value(std::unique_ptr<Value> val);
    void set_value(std::shared_ptr<Value> val);
    void set_value(Value& val);
    void set_value(Value&& val);
    std::shared_ptr<Value> get_value();
    std::shared_ptr<ArrayValue> get_array_value();
};

#endif // __VARIABLE_HPP__