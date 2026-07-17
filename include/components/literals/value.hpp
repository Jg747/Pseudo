#ifndef __VALUE_HPP__
#define __VALUE_HPP__

#include <string>
#include <memory>

class Value {
protected:
    std::string value;

public:
    Value();
    Value(const Value& val);
    Value(std::string value);
    Value(int value);
    Value(float value);
    Value(double value);
    virtual ~Value();

    virtual void set_value(std::string& val);
    virtual void set_value(Value& val);
    std::string get_value() const;

    virtual std::unique_ptr<Value> clone() const = 0;

    friend std::unique_ptr<Value> operator+(Value& val1, Value& val2);
    friend std::unique_ptr<Value> operator-(Value& val1, Value& val2);
    friend std::unique_ptr<Value> operator*(Value& val1, Value& val2);
    friend std::unique_ptr<Value> operator/(Value& val1, Value& val2);
    friend std::unique_ptr<Value> operator%(Value& val1, Value& val2);

    friend std::unique_ptr<Value> operator<(Value& val1, Value& val2);
    friend std::unique_ptr<Value> operator>(Value& val1, Value& val2);
    friend std::unique_ptr<Value> operator<=(Value& val1, Value& val2);
    friend std::unique_ptr<Value> operator>=(Value& val1, Value& val2);
    friend std::unique_ptr<Value> operator==(Value& val1, Value& val2);
    friend std::unique_ptr<Value> operator!=(Value& val1, Value& val2);

    friend std::unique_ptr<Value> operator&&(Value& val1, Value& val2);
    friend std::unique_ptr<Value> operator||(Value& val1, Value& val2);
};

#endif // __VALUE_HPP__