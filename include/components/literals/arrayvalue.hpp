#ifndef __ARRAY_VALUE__
#define __ARRAY_VALUE__

#include "value.hpp"

#include <memory>
#include <vector>

class ArrayValue : public Value {
private:
    std::vector<std::shared_ptr<Value>> array;

public:
    ArrayValue() {}
    ArrayValue(const Value& val);

    void add_value(std::unique_ptr<Value> val);
    void add_value(std::shared_ptr<Value> val);
    void add_value(Value& val);
    void add_value(Value&& val);

    void remove_value(int idx);

    int get_length();

    std::shared_ptr<Value> operator[](int idx);
    std::shared_ptr<Value> operator[](Value& idx);
    ArrayValue& operator=(Value& val);

    virtual std::unique_ptr<Value> clone() const override;
};

#endif // __ARRAY_VALUE__