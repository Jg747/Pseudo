#ifndef __NUMBERVALUE_HPP__
#define __NUMBERVALUE_HPP__

#include "value.hpp"
#include "stringvalue.hpp"

#include <string>

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
    NumberValue(double value);
    NumberValue(int value);
    NumberValue(std::string value);
    NumberValue(const Value& val);

    void set_value(std::string& val) override;
    void set_value(Value& val) override;
    double get_value() const;
    numbertype_e get_type() const;

    static bool is_number(std::string& str);

    static NumberValue add(Value& val1, Value& val2);
    static NumberValue sub(Value& val1, Value& val2);
    static NumberValue mul(Value& val1, Value& val2);
    static NumberValue div(Value& val1, Value& val2);
    static NumberValue mod(Value& val1, Value& val2);

    friend NumberValue operator+(NumberValue val1, NumberValue val2);
    friend NumberValue operator-(NumberValue val1, NumberValue val2);
    friend NumberValue operator*(NumberValue val1, NumberValue val2);
    friend NumberValue operator/(NumberValue val1, NumberValue val2);
    friend NumberValue operator%(NumberValue val1, NumberValue val2);

    friend NumberValue operator+(Value* val1, NumberValue val2);
    friend NumberValue operator-(Value* val1, NumberValue val2);
    friend NumberValue operator*(Value* val1, NumberValue val2);
    friend NumberValue operator/(Value* val1, NumberValue val2);
    friend NumberValue operator%(Value* val1, NumberValue val2);

    friend NumberValue operator+(NumberValue val1, Value* val2);
    friend NumberValue operator-(NumberValue val1, Value* val2);
    friend NumberValue operator*(NumberValue val1, Value* val2);
    friend NumberValue operator/(NumberValue val1, Value* val2);
    friend NumberValue operator%(NumberValue val1, Value* val2);

    friend NumberValue operator+(std::string val1, NumberValue val2);
    friend NumberValue operator-(std::string val1, NumberValue val2);
    friend NumberValue operator*(std::string val1, NumberValue val2);
    friend NumberValue operator/(std::string val1, NumberValue val2);
    friend NumberValue operator%(std::string val1, NumberValue val2);

    friend NumberValue operator+(NumberValue val1, std::string val2);
    friend NumberValue operator-(NumberValue val1, std::string val2);
    friend NumberValue operator*(NumberValue val1, std::string val2);
    friend NumberValue operator/(NumberValue val1, std::string val2);
    friend NumberValue operator%(NumberValue val1, std::string val2);

    friend NumberValue operator+(NumberValue val1, int val2);
    friend NumberValue operator-(NumberValue val1, int val2);
    friend NumberValue operator*(NumberValue val1, int val2);
    friend NumberValue operator/(NumberValue val1, int val2);
    friend NumberValue operator%(NumberValue val1, int val2);

    friend NumberValue operator+(int val1, NumberValue val2);
    friend NumberValue operator-(int val1, NumberValue val2);
    friend NumberValue operator*(int val1, NumberValue val2);
    friend NumberValue operator/(int val1, NumberValue val2);
    friend NumberValue operator%(int val1, NumberValue val2);

    friend NumberValue operator+(NumberValue val1, double val2);
    friend NumberValue operator-(NumberValue val1, double val2);
    friend NumberValue operator*(NumberValue val1, double val2);
    friend NumberValue operator/(NumberValue val1, double val2);
    friend NumberValue operator%(NumberValue val1, double val2);

    friend NumberValue operator+(double val1, NumberValue val2);
    friend NumberValue operator-(double val1, NumberValue val2);
    friend NumberValue operator*(double val1, NumberValue val2);
    friend NumberValue operator/(double val1, NumberValue val2);
    friend NumberValue operator%(double val1, NumberValue val2);

    friend NumberValue operator<(NumberValue val1, NumberValue val2);
    friend NumberValue operator>(NumberValue val1, NumberValue val2);
    friend NumberValue operator<=(NumberValue val1, NumberValue val2);
    friend NumberValue operator>=(NumberValue val1, NumberValue val2);
    friend NumberValue operator==(NumberValue val1, NumberValue val2);
    friend NumberValue operator!=(NumberValue val1, NumberValue val2);

    friend NumberValue operator<(Value* val1, NumberValue val2);
    friend NumberValue operator>(Value* val1, NumberValue val2);
    friend NumberValue operator<=(Value* val1, NumberValue val2);
    friend NumberValue operator>=(Value* val1, NumberValue val2);
    friend NumberValue operator==(Value* val1, NumberValue val2);
    friend NumberValue operator!=(Value* val1, NumberValue val2);

    friend NumberValue operator<(NumberValue val1, Value* val2);
    friend NumberValue operator>(NumberValue val1, Value* val2);
    friend NumberValue operator<=(NumberValue val1, Value* val2);
    friend NumberValue operator>=(NumberValue val1, Value* val2);
    friend NumberValue operator==(NumberValue val1, Value* val2);
    friend NumberValue operator!=(NumberValue val1, Value* val2);

    friend NumberValue operator<(NumberValue val1, std::string val2);
    friend NumberValue operator>(NumberValue val1, std::string val2);
    friend NumberValue operator<=(NumberValue val1, std::string val2);
    friend NumberValue operator>=(NumberValue val1, std::string val2);
    friend NumberValue operator==(NumberValue val1, std::string val2);
    friend NumberValue operator!=(NumberValue val1, std::string val2);

    friend NumberValue operator<(std::string val1, NumberValue val2);
    friend NumberValue operator>(std::string val1, NumberValue val2);
    friend NumberValue operator<=(std::string val1, NumberValue val2);
    friend NumberValue operator>=(std::string val1, NumberValue val2);
    friend NumberValue operator==(std::string val1, NumberValue val2);
    friend NumberValue operator!=(std::string val1, NumberValue val2);

    friend NumberValue operator<(NumberValue val1, int val2);
    friend NumberValue operator>(NumberValue val1, int val2);
    friend NumberValue operator<=(NumberValue val1, int val2);
    friend NumberValue operator>=(NumberValue val1, int val2);
    friend NumberValue operator==(NumberValue val1, int val2);
    friend NumberValue operator!=(NumberValue val1, int val2);

    friend NumberValue operator<(int val1, NumberValue val2);
    friend NumberValue operator>(int val1, NumberValue val2);
    friend NumberValue operator<=(int val1, NumberValue val2);
    friend NumberValue operator>=(int val1, NumberValue val2);
    friend NumberValue operator==(int val1, NumberValue val2);
    friend NumberValue operator!=(int val1, NumberValue val2);

    friend NumberValue operator<(NumberValue val1, double val2);
    friend NumberValue operator>(NumberValue val1, double val2);
    friend NumberValue operator<=(NumberValue val1, double val2);
    friend NumberValue operator>=(NumberValue val1, double val2);
    friend NumberValue operator==(NumberValue val1, double val2);
    friend NumberValue operator!=(NumberValue val1, double val2);

    friend NumberValue operator<(double val1, NumberValue val2);
    friend NumberValue operator>(double val1, NumberValue val2);
    friend NumberValue operator<=(double val1, NumberValue val2);
    friend NumberValue operator>=(double val1, NumberValue val2);
    friend NumberValue operator==(double val1, NumberValue val2);
    friend NumberValue operator!=(double val1, NumberValue val2);

    friend NumberValue operator&&(NumberValue val1, NumberValue val2);
    friend NumberValue operator&&(Value* val1, NumberValue val2);
    friend NumberValue operator&&(NumberValue val1, Value* val2);
    friend NumberValue operator&&(std::string val1, NumberValue val2);
    friend NumberValue operator&&(NumberValue val1, std::string val2);
    friend NumberValue operator&&(NumberValue val1, int val2);
    friend NumberValue operator&&(int val1, NumberValue val2);
    friend NumberValue operator&&(NumberValue val1, double val2);
    friend NumberValue operator&&(double val1, NumberValue val2);

    friend NumberValue operator||(NumberValue val1, NumberValue val2);
    friend NumberValue operator||(Value* val1, NumberValue val2);
    friend NumberValue operator||(NumberValue val1, Value* val2);
    friend NumberValue operator||(std::string val1, NumberValue val2);
    friend NumberValue operator||(NumberValue val1, std::string val2);
    friend NumberValue operator||(NumberValue val1, int val2);
    friend NumberValue operator||(int val1, NumberValue val2);
    friend NumberValue operator||(NumberValue val1, double val2);
    friend NumberValue operator||(double val1, NumberValue val2);

    NumberValue& operator=(Value& val);

    operator StringValue() const;
    operator bool() const;

    virtual std::unique_ptr<Value> clone() const override;
};

#endif // __NUMBERVALUE_HPP__