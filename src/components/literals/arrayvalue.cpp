#include "components/literals/arrayvalue.hpp"
#include "components/literals/numbervalue.hpp"

std::unique_ptr<Value> ArrayValue::clone() const {
    return std::make_unique<ArrayValue>(*this);
}

ArrayValue::ArrayValue(const Value& val) {
    std::string v = val.get_value();
    set_value(v);
}

std::shared_ptr<Value> ArrayValue::operator[](int idx) {
    std::size_t index = idx;
    if (index < 0 || index >= array.size()) {
        throw std::runtime_error("index out of bounds (" + std::to_string(index) + ")");
    }
    return array[index];
}

std::shared_ptr<Value> ArrayValue::operator[](Value& idx) {
    NumberValue v = (NumberValue) idx;
    if (v.get_type() != numbertype_e::Integer) {
        throw std::runtime_error("index not an integer");
    }
    
    int val = (int) v.get_value();
    if (val < 0 || (size_t) val > array.size()) {
        throw std::runtime_error("index out of bounds (index: " + std::to_string((int) v.get_value()) + ", len: " + std::to_string(array.size()) + ")");
    }
    return array[val];
}

void ArrayValue::add_value(Value& val) {
    array.push_back(val.clone());
}

void ArrayValue::add_value(Value&& val) {
    add_value(static_cast<Value&>(val));
}

void ArrayValue::add_value(std::unique_ptr<Value> val) {
    array.push_back(std::move(val));
}

void ArrayValue::add_value(std::shared_ptr<Value> val) {
    array.push_back(val);
}

void ArrayValue::remove_value(int idx) {
    std::size_t index = idx;
    if (index < 0 || index >= array.size()) {
        throw std::runtime_error("index out of bounds (" + std::to_string(index) + ")");
    }
    array.erase(array.begin() + index, array.begin() + index + 1);
}

int ArrayValue::get_length() {
    return (int) array.size();
}

ArrayValue& ArrayValue::operator=(Value& val) {
    if (this != &val) {
        ArrayValue v = (ArrayValue) val;
        this->array = v.array;
    }
    return *this;
}