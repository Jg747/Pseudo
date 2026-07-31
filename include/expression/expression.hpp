#ifndef __EXPRESSION_HPP__
#define __EXPRESSION_HPP__

#include <vector>
#include <string>
#include <stack>
#include <memory>

#include "lexer.hpp"
#include "variablecontext.hpp"

class Expression {
public:
    Expression() {}
    Expression(std::vector<Token>& tokens);
    Expression(std::vector<Token>&& tokens);
    
    std::vector<Token>& rpn() { return tokens; }
    std::vector<std::string>& req_vars() { return var_list; }

    static Expression parse_expression(const std::string& expression);
    std::shared_ptr<Value> evaluate(VariableContext& context);
    void validate() const;

private:
    std::vector<Token> tokens;
    std::vector<std::string> var_list;

    void operation(std::stack<std::shared_ptr<Value>>& values, const Token& token) const;
    void assign_deep_index(ArrayValue* value, std::shared_ptr<Value>& val, std::vector<int> indexes, size_t depth);
};

#endif // __EXPRESSION_HPP__