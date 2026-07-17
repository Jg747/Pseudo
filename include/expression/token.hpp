#ifndef __TOKEN_HPP__
#define __TOKEN_HPP__

#include <string>
#include <unordered_map>

enum class token_t {
    Number,
    String,
    Identifier,

    Plus,
    Minus,
    UnaryMinus,
    Multiply,
    Divide,
    Modulo,

    LeftParen,
    RightParen,
    LeftBracket,
    RightBracket,

    Less,
    LessEqual,
    Greater,
    GreaterEqual,
    Equal,
    NotEqual,
    LogicalAnd,
    LogicalOr,
    
    ArraySize,
    ArrayIndex
};

enum class op_pos {
    Prefix,
    Infix,
    Postfix
};

struct op_info {
    int precedence;
    bool rightAssociative;
    int operandCount;
    op_pos position;
};

class Token {
public:
    token_t type;

    std::string text;
    double value = 0.0;
    std::size_t position = 0;

    Token() {};
    Token(std::string& s);
    Token(std::string&& s);
    Token(std::string&& s, bool string);
    Token(std::string& s, bool string);
    Token(double d);
    Token(token_t t) : type(t) {}

    std::string print() const;
    friend std::ostream& operator<<(std::ostream& os, const Token& c);

    static const op_info* get_op_info(token_t type);
};

#endif // __TOKEN_HPP__