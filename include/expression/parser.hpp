#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <vector>

#include "token.hpp"
#include "expression.hpp"

enum class ParserState {
    ExpectOperand,
    ExpectOperator
};

class Parser {
private:
    std::vector<Token> output;
    std::stack<Token> operators;
    std::stack<int> call_op_counts;
    ParserState state;

    void push_operator(const Token& token);
    void operator_position(const Token& token, const op_info* op);
    void operand(const Token& token);
    void left_paren();
    void right_paren();
    void separator();
    void left_brack();
    void right_brack();
    void check_operators();

public:
    Expression parse(const std::vector<Token>& tokens);
};

#endif // __PARSER_HPP__