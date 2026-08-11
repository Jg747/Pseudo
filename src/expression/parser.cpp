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

        bool pop = (!current->right_associative && current->precedence <= top->precedence) ||
                   (current->right_associative && current->precedence < top->precedence);

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

void Parser::left_paren() {
    if (state == ParserState::ExpectOperand) {
        operators.push(Token(token_t::LeftParen));
    } else if (state == ParserState::ExpectOperator) {
        push_operator(Token(token_t::FunctionCall));
        operators.push(Token(token_t::LeftParen));
        
        state = ParserState::ExpectOperand;
        call_op_counts.push(0);
        (output.end() - 1)->text.push_back('\x01');
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

    if (!operators.empty() && operators.top().type == token_t::FunctionCall) {
        if (call_op_counts.empty()) {
            throw std::runtime_error("'(' wasn't Call operator");
        }
        operators.top().operands = call_op_counts.top() + (state == ParserState::ExpectOperator);
        call_op_counts.pop();

        state = ParserState::ExpectOperator;
    }
}

void Parser::separator() {
    if (state != ParserState::ExpectOperator) {
        throw std::runtime_error("Unexpected separator");
    }

    while (!operators.empty() && operators.top().type != token_t::LeftParen) {
        output.push_back(operators.top());
        operators.pop();
    }

    if (operators.empty()) {
        throw std::runtime_error("Unexpected separator");
    }

    call_op_counts.top()++;
    state = ParserState::ExpectOperand;
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
            left_paren();
            continue;
        }

        if (token.type == token_t::RightParen) {
            right_paren();
            continue;
        }

        if (token.type == token_t::Separator) {
            separator();
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