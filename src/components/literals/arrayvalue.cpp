#include "components/literals/arrayvalue.hpp"
#include "components/literals/numbervalue.hpp"

std::unique_ptr<Value> ArrayValue::clone() const {
    return std::make_unique<ArrayValue>(*this);
}

ArrayValue::ArrayValue(const Value& val) {
    if (dynamic_cast<const ArrayValue*>(&val)) {
        this->array = ((ArrayValue*) &val)->array;
    } else {
        array.push_back(val.clone());
    }
}

std::shared_ptr<Value>& ArrayValue::operator[](int idx) {
    std::size_t index = idx;
    if (index < 0 || index >= array.size()) {
        throw std::runtime_error("index out of bounds (" + std::to_string(index) + ")");
    }
    return array[index];
}

std::shared_ptr<Value>& ArrayValue::operator[](Value& idx) {
    NumberValue v = (NumberValue) idx;
    if (v.get_type() != numbertype_e::Integer) {
        throw std::runtime_error("index not an integer");
    }
    
    int val = v.get_int_value();
    if (val < 0 || (size_t) val >= array.size()) {
        throw std::runtime_error("index out of bounds (index: " + std::to_string(v.get_int_value()) + ", len: " + std::to_string(array.size()) + ")");
    }
    return array[val];
}

void ArrayValue::add_value(Value& val) {
    val.set_assign_array(this);
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
        ArrayValue* v = (ArrayValue*) &val;
        this->array = v->array;
    }
    return *this;
}

ArrayValue operator+(ArrayValue& val1, Value& val2) {
    if (dynamic_cast<ArrayValue*>(&val2)) {
        ArrayValue* arr = (ArrayValue*) &val2;
        for (int i = 0; i < arr->get_length(); i++) {
            val1.add_value((*arr)[i]);
        }
        return val1;
    }
    val1.add_value(val2);
    return val1;
}

ArrayValue operator+(Value& val1, ArrayValue& val2) {
    if (dynamic_cast<ArrayValue*>(&val1)) {
        ArrayValue* arr = (ArrayValue*) &val1;
        for (int i = 0; i < val2.get_length(); i++) {
            arr->add_value(val2[i]);
        }
        return *((ArrayValue*) &val1);
    }
    val2.array.insert(val2.array.begin(), val1.clone());
    return val2;
}

ArrayValue operator-(ArrayValue& val1, Value& val2) {
    NumberValue number(val2);

    if (number.get_type() != numbertype_e::Integer) {
        throw std::runtime_error("index not integer (" + number.get_value() + ")");
    }

    size_t index = number.get_int_value();
    if (index < 0 || index >= val1.array.size()) {
        throw std::runtime_error("index out of bounds (" + std::to_string(index) + ")");
    }

    val1.array.erase(val1.array.begin() + index);
    return val1;
}