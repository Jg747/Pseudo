#ifndef __EXPRESSION_HPP__
#define __EXPRESSION_HPP__

#include <vector>
#include <string>
#include <stack>

#include "lexer.hpp"
#include "variablecontext.hpp"

class Expression {
public:
    Expression(std::vector<Token>& tokens) : tokens(tokens) {}
    Expression(std::vector<Token>&& tokens) : tokens(tokens) {}
    
    std::vector<Token>& rpn() { return tokens; }

    static Expression parse_expression(const std::string& expression);
    std::shared_ptr<Value> evaluate(const VariableContext& context) const;
    void validate() const;
private:
    std::vector<Token> tokens;

    void operation(std::stack<std::shared_ptr<Value>>& values, const Token& token) const;
};

#endif // __EXPRESSION_HPP__