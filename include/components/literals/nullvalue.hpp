#ifndef __NULL_VALUE_HPP__
#define __NULL_VALUE_HPP__

#include "value.hpp"

#include <memory>

class NullValue : public Value {
public:
    virtual std::unique_ptr<Value> clone() const override {
        return std::make_unique<NullValue>(*this);
    }
};

#endif /* __NULL_VALUE_HPP__ */