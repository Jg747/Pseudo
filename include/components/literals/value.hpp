#ifndef __VALUE_HPP__
#define __VALUE_HPP__

#include <string>
#include <memory>

class Value {
protected:
    std::string value;

public:
    Value();
    Value(std::string value);
    Value(int value);
    Value(float value);
    Value(double value);
    virtual ~Value();

    virtual void abstract() = 0;
};

#endif // __VALUE_HPP__