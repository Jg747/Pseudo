#ifndef __VALUE_HPP__
#define __VALUE_HPP__

#include <string>
#include <memory>

class Variable;
class ArrayValue;

typedef union {
    Variable* var;
    ArrayValue* arr;
} assignment;

class Value {
protected:
    std::string value;
    assignment assign;
    bool assignment_var;

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

    bool assigment_type() const;

    void set_variable(Variable* var);
    Variable* get_variable() const;

    void set_assign_array(ArrayValue* arr);
    ArrayValue* get_assign_array() const;

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