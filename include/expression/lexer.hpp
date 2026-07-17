#ifndef __LEXER_HPP__
#define __LEXER_HPP__

#include "token.hpp"

#include <string>
#include <vector>
#include <optional>

class Lexer {
public:
    std::vector<Token> tokenize(const std::string& expression);

private:
    std::string expression;
    std::vector<Token> tokens;
    std::size_t i;
    char c;

    void process_number();
    void process_identifier();
    void process_special();
    void process_string();

    bool isIdentifierStart(char c) const;
    bool isIdentifier(char c) const;
    void replace_all(std::string& str, std::string sub, std::string with) const;
};

#endif // __LEXER_HPP__