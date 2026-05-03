#ifndef __NUMBERVALUE_HPP__
#define __NUMBERVALUE_HPP__

#include "value.hpp"
#include "numbervalue.hpp"

#include <string>
#include <memory>

class NumberValue;

class StringValue : public Value {
private:
    int len;
public:
    StringValue();
    StringValue(std::string& value);
    StringValue(Value& val);

    void set_value(std::string& value);
    void set_value(Value& value);
    std::string get_value() const;
    int get_len() const;

    static StringValue concat(Value& val1, Value& val2);
    static StringValue subst(Value& val, int start, int count, StringValue& string);

    friend StringValue operator+(StringValue& val1, StringValue& val2);
    operator NumberValue() const;
};

#endif // __NUMBERVALUE_HPP__