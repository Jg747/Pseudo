#include "expression/parser.hpp"
#include "expression/token.hpp"
#include "expression/expression.hpp"

#include <stack>
#include <stdexcept>

void Parser::push_operator(const Token& token) {
    const op_info* current = Token::get_op_info(token.type);

    while (!operators.empty()) {
        const op_info* top = Token::get_op_info(operators.top().type);
        if (top == nullptr) {
            break;
        }

        bool pop = (!current->rightAssociative && current->precedence <= top->precedence) ||
                   (current->rightAssociative && current->precedence < top->precedence);

        if (!pop) {
            break;
        }

        output.push_back(operators.top());
        operators.pop();
    }

    operators.push(token);
}

void Parser::operator_position(const Token& token, const op_info* op) {
    switch (op->position) {
        case op_pos::Prefix: {
            if (state != ParserState::ExpectOperand) {
                throw std::runtime_error("Unexpected prefix operator ('" + token.print() + "')");
            }
            push_operator(token);
            break;
        }
        case op_pos::Infix: {
            if (state != ParserState::ExpectOperator) {
                throw std::runtime_error("Unexpected operator");
            }
            push_operator(token);
            state = ParserState::ExpectOperand;
            break;
        }

        case op_pos::Postfix: {
            if (state != ParserState::ExpectOperator) {
                throw std::runtime_error("Unexpected postfix operator");
            }
            push_operator(token);
            break;
        }
    }
}

void Parser::right_paren() {
    while (!operators.empty() && operators.top().type != token_t::LeftParen) {
        output.push_back(operators.top());
        operators.pop();
    }
            
    if (operators.empty()) {
        throw std::runtime_error("Missing '('");
    }
            
    operators.pop();
}

void Parser::left_brack() {
    if (state != ParserState::ExpectOperator) {
        throw std::runtime_error("Unexpected '['");
    }
            
    push_operator(Token(token_t::ArrayIndex));
            
    operators.push(Token(token_t::LeftParen));
    state = ParserState::ExpectOperand;
}

void Parser::right_brack() {
    while (!operators.empty() && operators.top().type != token_t::LeftParen) {
        output.push_back(operators.top());
        operators.pop();
    }

    if (operators.empty()) {
        throw std::runtime_error("Missing '['");
    }

    operators.pop();
    state = ParserState::ExpectOperator;
}

void Parser::check_operators() {
    while (!operators.empty()) {
        if (operators.top().type == token_t::LeftParen) {
            throw std::runtime_error("Missing ')'");
        }

        output.push_back(operators.top());
        operators.pop();
    }
}

void Parser::operand(const Token& token) {
    if (state != ParserState::ExpectOperand) {
        throw std::runtime_error("Unexpected operand");
    }
    output.push_back(token);
    state = ParserState::ExpectOperator;
}

Expression Parser::parse(const std::vector<Token>& tokens) {
    output.clear();
    std::stack<Token>().swap(operators);

    state = ParserState::ExpectOperand;

    for (const Token& token : tokens) {
        if (token.type == token_t::Number || token.type == token_t::String || token.type == token_t::Identifier) {
            operand(token);
            continue;
        }

        const op_info* op = Token::get_op_info(token.type);
        if (op) {
            operator_position(token, op);
            continue;
        }

        if (token.type == token_t::LeftParen) {
            operators.push(token);
            continue;
        }

        if (token.type == token_t::RightParen) {
            right_paren();
            continue;
        }

        if (token.type == token_t::LeftBracket) {
            left_brack();
            continue;
        }

        if (token.type == token_t::RightBracket) {
            right_brack();
            continue;
        }

        throw std::runtime_error("Unexpected token");
    }

    check_operators();

    Expression ret(std::move(output));
    ret.validate();
    
    return ret;
}