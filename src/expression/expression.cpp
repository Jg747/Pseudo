#include "expression/expression.hpp"
#include "expression/lexer.hpp"
#include "expression/parser.hpp"
#include "components/literals/value.hpp"
#include "components/literals/arrayvalue.hpp"
#include "components/literals/stringvalue.hpp"
#include "components/literals/numbervalue.hpp"
#include "components/literals/nullvalue.hpp"
#include "lang.hpp"

#include <stack>
#include <cmath>
#include <vector>
#include <memory>

Expression::Expression(std::vector<Token>& tokens) : tokens(tokens) {
    for (auto& t : tokens) {
        if (t.type == token_t::Identifier) {
            var_list.push_back(t.text);
        }
    }
}

Expression::Expression(std::vector<Token>&& tokens) : tokens(tokens) {
    for (auto& t : tokens) {
        if (t.type == token_t::Identifier) {
            var_list.push_back(t.text);
        }
    }
}

void Expression::init() {
    assign_var = "";
    assign = false;
    begin_str = false;
    arr = nullptr;
    init_arr = -1;

    indexing_depth = std::vector<int>();
}

void Expression::index_op(std::stack<std::shared_ptr<Value>>& values) {
    NumberValue index = *values.top();
    i = index.get_int_value();
    values.pop();
    indexing_depth.push_back(i);

    if (dynamic_cast<ArrayValue*>(values.top().get()) && !begin_str) {
        ArrayValue array = *((ArrayValue*) values.top().get());
        values.pop();
        values.push(array[index]);
    } else if (dynamic_cast<StringValue*>(values.top().get())) {
        StringValue str = *((StringValue*) values.top().get());
        values.pop();
        values.push(str[index]);
    } else if (dynamic_cast<NullValue*>(values.top().get()) || begin_str) {
        Variable* var = values.top()->get_variable();
        values.pop();
        ArrayValue array;
        if (index >= 0) {
            array.add_value(NullValue());
            while (index.get_int_value() >= array.get_length()) {
                array.add_value(NullValue());
            }

            values.push(array[index]);
            init_arr = i;
        } else {
            values.push(array.clone());
        }
                    
        var->set_value(array);
        if (init_arr != -1) {
            arr = var->get_array_value().get();
        }
    } else {
        throw std::runtime_error("Can't apply index to this type of value");
    }

    assign = false;
}

void Expression::assign_op(std::stack<std::shared_ptr<Value>>& values, VariableContext& context) {
    std::shared_ptr<Value> rhs = values.top();
    values.pop();

    std::shared_ptr<Value> lhs = values.top();
    values.pop();

    if (dynamic_cast<ArrayValue*>(rhs.get())) {
        lhs = ArrayValue(*rhs).clone();
    } else if (dynamic_cast<NumberValue*>(rhs.get())) {
        lhs = NumberValue(*rhs).clone();
    } else {
        lhs = StringValue(*rhs).clone();
    }
                
    values.push(lhs);
    if (assign) {
        context.get_var(assign_var)->set_value(lhs);
    } else if (i >= 0) {
        Value* var = context.get_var(assign_var)->get_value().get();
        if (dynamic_cast<ArrayValue*>(var)) {
            // (*((ArrayValue*) var))[i] = lhs; //
            assign_deep_index((ArrayValue*) var, lhs, indexing_depth, 0);
        } else if (dynamic_cast<StringValue*>(var)) {
            StringValue* v = (StringValue*) var;
            std::string s = v->get_value();
            std::string new_val = lhs->get_value();
            if (new_val.length() > 1) {
                throw std::runtime_error("Invalid value provided ('" + new_val + "')");
            }
            if (new_val.length() == 0) {
                s.erase(s.begin() + i);
            } else {
                s[i] = new_val[0];
            }
            v->set_value(s);
        } else {
            throw std::runtime_error("Can't apply index to this type of value");
        }
    }
}

void Expression::identifier_op(const Token& token, std::stack<std::shared_ptr<Value>>& values, VariableContext& context) {
    if (token.text == BEGIN_STR) {
        values.push(NumberValue(-1).clone());
        begin_str = true;
        return;
    }

    auto value = context.get(token.text);
    if (!value) {
        throw std::runtime_error("Undefined variable");
    }
                
    if (!assign) {
        assign = true;
        if (assign_var == "") {
            assign_var = token.text;
        } else {
            assign = false;
        }
    }

    values.push(value);
}

void Expression::unaryminus_op(std::stack<std::shared_ptr<Value>>& values) {
    if (values.empty()) {
        throw std::runtime_error("Missing operand.");
    }

    std::shared_ptr<Value> value = values.top();
    values.pop();

    values.push((NumberValue(0) - *value).clone());
}

void Expression::arraysize_op(std::stack<std::shared_ptr<Value>>& values) {
    if (dynamic_cast<ArrayValue*>(values.top().get())) {
        ArrayValue array = *((ArrayValue*) values.top().get());
        values.pop();
        values.push(NumberValue(array.get_length()).clone());
    } else if (dynamic_cast<StringValue*>(values.top().get())) {
        StringValue str = *((StringValue*) values.top().get());
        values.pop();
        values.push(NumberValue(str.get_len()).clone());
    } else {
        throw std::runtime_error("Can't apply size to this type of value");
    }
}

void Expression::init_copy_array(std::stack<std::shared_ptr<Value>>& values) {
    /*
        type:
            0: String
            1: Array
            2: Number
    */
    
    int type;
    Value* v = values.top().get();

    if (dynamic_cast<StringValue*>(v)) {
        type = 0;
    } else if (dynamic_cast<ArrayValue*>(v)) {
        type = 1;
    } else if (dynamic_cast<NumberValue*>(v)) {
        type = 2;
    } else {
        throw std::runtime_error("Invalid value provided in array copy initialization");
    }

    for (int i = 0; i < init_arr; i++) {
        switch (type) {
            case 0:
                (*arr)[i] = StringValue(*((StringValue*) v)).clone();
                break;
            case 1:
                (*arr)[i] = ArrayValue(*((ArrayValue*) v)).clone();
                break;
            case 2:
                (*arr)[i] = NumberValue(*((NumberValue*) v)).clone();
                break;
            default:
                break;
        }
    }
}

std::shared_ptr<Value> Expression::evaluate(VariableContext& context) {
    std::stack<std::shared_ptr<Value>> values;
    init();

    for (const Token& token : tokens) {
        switch (token.type) {
            case token_t::Number:
                values.push(std::make_shared<NumberValue>(token.value));
                break;
            case token_t::String:
                values.push(std::make_shared<StringValue>(token.text));
                break;
            case token_t::Identifier: {
                identifier_op(token, values, context);
                break;
            }
            case token_t::UnaryMinus: {
                unaryminus_op(values);
                break;
            }
            case token_t::Plus:
            case token_t::Minus:
            case token_t::Multiply:
            case token_t::Divide:
            case token_t::Modulo:
            case token_t::Less:
            case token_t::LessEqual:
            case token_t::Greater:
            case token_t::GreaterEqual:
            case token_t::Equal:
            case token_t::NotEqual:
            case token_t::LogicalAnd:
            case token_t::LogicalOr:
                operation(values, token);
                break;
            case token_t::ArrayIndex: {
                index_op(values);
                break;
            }
            case token_t::ArraySize: {
                arraysize_op(values);
                break;
            }
            case token_t::Assign: {
                assign_op(values, context);
                break;
            }
            default:
                throw std::runtime_error("Invalid RPN token.");
        }
    }

    if (init_arr != -1) {
        init_copy_array(values);
    }

    if (values.size() != 1) {
        throw std::runtime_error("Invalid expression.");
    }

    return values.top();
}

void Expression::operation(std::stack<std::shared_ptr<Value>>& values, const Token& token) const {
    if (values.size() < 2) {
        throw std::runtime_error("Missing operand.");
    }

    std::shared_ptr<Value> rhs = values.top();
    values.pop();

    std::shared_ptr<Value> lhs = values.top();
    values.pop();

    switch (token.type) {
        case token_t::Plus:
            values.push((*lhs + *rhs));
            break;
        case token_t::Minus:
            values.push((*lhs - *rhs));
            break;
        case token_t::Multiply:
            values.push((*lhs * *rhs));
            break;
        case token_t::Divide:
            if (((NumberValue) *rhs) == 0.0) {
                throw std::runtime_error("Division by zero.");
            }
            values.push((*lhs / *rhs));
            break;
        case token_t::Modulo:
            if (((NumberValue) *rhs) == 0.0) {
                throw std::runtime_error("Division by zero.");
            }
            values.push((*lhs % *rhs));
            break;
        case token_t::Less:
            values.push((*lhs < *rhs));
            break;
        case token_t::LessEqual:
            values.push((*lhs <= *rhs));
            break;
        case token_t::Greater:
            values.push((*lhs > *rhs));
            break;
        case token_t::GreaterEqual:
            values.push((*lhs >= *rhs));
            break;
        case token_t::Equal:
            values.push((*lhs == *rhs));
            break;
        case token_t::NotEqual:
            values.push((*lhs != *rhs));
            break;
        case token_t::LogicalAnd:
            values.push((*lhs && *rhs));
            break;
        case token_t::LogicalOr:
            values.push((*lhs || *rhs));
            break;
        default:
            break;
    }
}

void Expression::validate() const {
    int stackDepth = 0;

    for (const Token& token : tokens) {
        switch (token.type) {
            case token_t::Number:
            case token_t::String:
            case token_t::Identifier:
                stackDepth++;
                break;
            default: {
                const op_info* op = Token::get_op_info(token.type);

                if (!op) {
                    throw std::runtime_error("Unexpected token.");
                }

                if (stackDepth < op->operandCount)
                    throw std::runtime_error("Missing operand.");

                stackDepth -= op->operandCount;
                stackDepth++;
                break;
            }
        }
    }

    if (stackDepth != 1) {
        throw std::runtime_error("Malformed expression.");
    }
}

Expression Expression::parse_expression(const std::string& expression) {
    Lexer l;
    std::vector<Token> tokens = l.tokenize(expression);
    Parser p;
    return p.parse(tokens);
}

void Expression::assign_deep_index(ArrayValue* value, std::shared_ptr<Value>& val, std::vector<int> indexes, size_t depth) {
    if (depth == indexes.size() - 1) {
        (*value)[indexes[depth]] = val;
        return;
    }
    assign_deep_index((ArrayValue*) (*value)[indexes[depth]].get(), val, indexes, depth + 1);
}