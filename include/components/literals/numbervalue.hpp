#ifndef __NUMBERVALUE_HPP__
#define __NUMBERVALUE_HPP__

#include "value.hpp"
#include "stringvalue.hpp"

#include <string>
#include <memory>

class StringValue;

enum numbertype_e {
    Integer,
    Double
};

class NumberValue : public Value {
private:
    numbertype_e type;
public:
    NumberValue();
    NumberValue(std::string& value);
    NumberValue(Value& val);

    void set_value(std::string& value);
    void set_value(Value& value);
    int get_value() const;
    numbertype_e get_type() const;

    static NumberValue add(Value& val1, Value& val2);
    static NumberValue sub(Value& val1, Value& val2);
    static NumberValue mul(Value& val1, Value& val2);
    static NumberValue div(Value& val1, Value& val2);
    static NumberValue mod(Value& val1, Value& val2);

    friend NumberValue operator+(NumberValue& val1, NumberValue& val2);
    friend NumberValue operator-(NumberValue& val1, NumberValue& val2);
    friend NumberValue operator*(NumberValue& val1, NumberValue& val2);
    friend NumberValue operator/(NumberValue& val1, NumberValue& val2);
    friend NumberValue operator%(NumberValue& val1, NumberValue& val2);

    operator StringValue() const;
};

#endif // __NUMBERVALUE_HPP__