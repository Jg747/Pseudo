#include "expression/expression.hpp"
#include "expression/lexer.hpp"
#include "expression/parser.hpp"
#include "components/literals/value.hpp"
#include "components/literals/arrayvalue.hpp"
#include "components/literals/stringvalue.hpp"
#include "components/literals/numbervalue.hpp"

#include <stack>
#include <cmath>

std::shared_ptr<Value> Expression::evaluate(const VariableContext& context) const {
    std::stack<std::shared_ptr<Value>> values;

    for (const Token& token : tokens) {
        switch (token.type) {
            case token_t::Number:
                values.push(std::make_shared<NumberValue>(token.value));
                break;
            case token_t::String:
                values.push(std::make_shared<StringValue>(token.text));
                break;
            case token_t::Identifier: {
                auto value = context.get(token.text);
                if (!value) {
                    throw std::runtime_error("Undefined variable");
                }
                values.push(value);
                break;
            }
            case token_t::UnaryMinus: {
                if (values.empty()) {
                    throw std::runtime_error("Missing operand.");
                }

                std::shared_ptr<Value> value = values.top();
                values.pop();

                values.push((NumberValue(0) - *value).clone());
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
                NumberValue index = *values.top();
                values.pop();

                if (dynamic_cast<ArrayValue*>(values.top().get())) {
                    ArrayValue array = *((ArrayValue*) values.top().get());
                    values.pop();
                    values.push(array[index]);
                } else if (dynamic_cast<StringValue*>(values.top().get())) {
                    StringValue str = *((StringValue*) values.top().get());
                    values.pop();
                    values.push(str[index]);
                }
                break;
            }
            case token_t::ArraySize: {
                if (dynamic_cast<ArrayValue*>(values.top().get())) {
                    ArrayValue array = *((ArrayValue*) values.top().get());
                    values.pop();
                    values.push(NumberValue(array.get_length()).clone());
                } else if (dynamic_cast<StringValue*>(values.top().get())) {
                    StringValue str = *((StringValue*) values.top().get());
                    values.pop();
                    values.push(NumberValue(str.get_len()).clone());
                }
                break;
            }
            default:
                throw std::runtime_error("Invalid RPN token.");
        }
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