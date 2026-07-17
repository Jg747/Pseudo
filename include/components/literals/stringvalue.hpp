#ifndef __STRINGVALUE_HPP__
#define __STRINGVALUE_HPP__

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
    StringValue(std::string value);
    StringValue(const Value& val);

    void set_value(std::string& val) override;
    void set_value(Value& value) override;
    int get_len() const;

    static StringValue concat(Value& val1, Value& val2);
    static StringValue concat(Value& val1, std::string val2);
    static StringValue concat(std::string val1, Value& val2);
    static StringValue subst(Value& val, int start, int count, StringValue& string);
    static StringValue subst(Value& val, int start, int count, std::string string);

    friend StringValue operator+(StringValue val1, StringValue val2);
    friend StringValue operator+(StringValue val1, std::string val2);
    friend StringValue operator+(std::string val1, StringValue val2);
    friend StringValue operator+(StringValue val1, Value* val2);
    friend StringValue operator+(Value* val1, StringValue val2);

    friend NumberValue operator<(StringValue val1, StringValue val2);
    friend NumberValue operator>(StringValue val1, StringValue val2);
    friend NumberValue operator<=(StringValue val1, StringValue val2);
    friend NumberValue operator>=(StringValue val1, StringValue val2);
    friend NumberValue operator==(StringValue val1, StringValue val2);
    friend NumberValue operator!=(StringValue val1, StringValue val2);

    friend NumberValue operator<(StringValue val1, Value* val2);
    friend NumberValue operator>(StringValue val1, Value* val2);
    friend NumberValue operator<=(StringValue val1, Value* val2);
    friend NumberValue operator>=(StringValue val1, Value* val2);
    friend NumberValue operator==(StringValue val1, Value* val2);
    friend NumberValue operator!=(StringValue val1, Value* val2);

    friend NumberValue operator<(Value* val1, StringValue val2);
    friend NumberValue operator>(Value* val1, StringValue val2);
    friend NumberValue operator<=(Value* val1, StringValue val2);
    friend NumberValue operator>=(Value* val1, StringValue val2);
    friend NumberValue operator==(Value* val1, StringValue val2);
    friend NumberValue operator!=(Value* val1, StringValue val2);

    friend NumberValue operator<(StringValue val1, std::string val2);
    friend NumberValue operator>(StringValue val1, std::string val2);
    friend NumberValue operator<=(StringValue val1, std::string val2);
    friend NumberValue operator>=(StringValue val1, std::string val2);
    friend NumberValue operator==(StringValue val1, std::string val2);
    friend NumberValue operator!=(StringValue val1, std::string val2);

    friend NumberValue operator<(std::string val1, StringValue val2);
    friend NumberValue operator>(std::string val1, StringValue val2);
    friend NumberValue operator<=(std::string val1, StringValue val2);
    friend NumberValue operator>=(std::string val1, StringValue val2);
    friend NumberValue operator==(std::string val1, StringValue val2);
    friend NumberValue operator!=(std::string val1, StringValue val2);

    friend NumberValue operator&&(StringValue val1, StringValue val2);
    friend NumberValue operator&&(StringValue val1, Value* val2);
    friend NumberValue operator&&(Value* val1, StringValue val2);
    friend NumberValue operator&&(StringValue val1, std::string val2);
    friend NumberValue operator&&(std::string val1, StringValue val2);

    friend NumberValue operator||(StringValue val1, StringValue val2);
    friend NumberValue operator||(StringValue val1, Value* val2);
    friend NumberValue operator||(Value* val1, StringValue val2);
    friend NumberValue operator||(StringValue val1, std::string val2);
    friend NumberValue operator||(std::string val1, StringValue val2);

    std::shared_ptr<Value> operator[](Value& idx);
    StringValue& operator=(Value& val);
    
    operator NumberValue() const;

    virtual std::unique_ptr<Value> clone() const override;
};

#endif // __STRING_VALUE__