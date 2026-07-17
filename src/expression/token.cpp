#include "expression/token.hpp"

#include <string>
#include <unordered_map>

Token::Token(std::string& s) {
    text = s;
    type = token_t::Identifier;
}

Token::Token(std::string&& s) {
    text = s;
    type = token_t::Identifier;
}

Token::Token(std::string& s, bool string) {
    text = s;
    type = token_t::Identifier;
    if (string) {
        type = token_t::String;
    } else {
        type = token_t::Identifier;
    }
}

Token::Token(std::string&& s, bool string) {
    text = s;
    if (string) {
        type = token_t::String;
    } else {
        type = token_t::Identifier;
    }
}

Token::Token(double d) {
    value = d;
    type = token_t::Number;
}

std::string Token::print() const {
    switch (type) {
        case token_t::Number:
            return "Number";
        case token_t::String:
            return "String";
        case token_t::Identifier:
            return "Identifier";
        case token_t::Plus:
            return "Plus";
        case token_t::Minus:
            return "Minus";
        case token_t::UnaryMinus:
            return "UnaryMinus";
        case token_t::Multiply:
            return "Multiply";
        case token_t::Divide:
            return "Divide";
        case token_t::Modulo:
            return "Modulo";
        case token_t::LeftParen:
            return "LeftParen";
        case token_t::RightParen:
            return "RightParen";
        case token_t::LeftBracket:
            return "LeftBracket";
        case token_t::RightBracket:
            return "RightBracket";
        case token_t::Less:
            return "Less";
        case token_t::LessEqual:
            return "LessEqual";
        case token_t::Greater:
            return "Greater";
        case token_t::GreaterEqual:
            return "GreaterEqual";
        case token_t::Equal:
            return "Equal";
        case token_t::NotEqual:
            return "NotEqual";
        case token_t::LogicalAnd:
            return "LogicalAnd";
        case token_t::LogicalOr:
            return "LogicalOr";
        case token_t::ArraySize:
            return "ArraySize";
        case token_t::ArrayIndex:
            return "ArrayIndex";
        default:
            return "Unknown";
    }
}

std::ostream& operator<<(std::ostream& os, const Token& c) {
    os << c.print();
    return os;
}



const op_info* Token::get_op_info(token_t type) {
    static const std::unordered_map<token_t, op_info> table = {
        { token_t::ArraySize,    {9, true, 1, op_pos::Postfix}},
        { token_t::ArrayIndex,   {9, false, 2, op_pos::Infix} },

        { token_t::UnaryMinus,   {8, true, 1, op_pos::Prefix} },

        { token_t::Multiply,     {7, false, 2, op_pos::Infix} },
        { token_t::Divide,       {7, false, 2, op_pos::Infix} },
        { token_t::Modulo,       {7, false, 2, op_pos::Infix} },

        { token_t::Plus,         {6, false, 2, op_pos::Infix} },
        { token_t::Minus,        {6, false, 2, op_pos::Infix} },

        { token_t::Less,         {5, false, 2, op_pos::Infix} },
        { token_t::LessEqual,    {5, false, 2, op_pos::Infix} },
        { token_t::Greater,      {5, false, 2, op_pos::Infix} },
        { token_t::GreaterEqual, {5, false, 2, op_pos::Infix} },

        { token_t::Equal,        {4, false, 2, op_pos::Infix} },
        { token_t::NotEqual,     {4, false, 2, op_pos::Infix} },

        { token_t::LogicalAnd,   {3, false, 2, op_pos::Infix} },
        { token_t::LogicalOr,    {3, false, 2, op_pos::Infix} }
    };

    if (table.contains(type)) {
        return &table.at(type);
    }
    return nullptr;
}