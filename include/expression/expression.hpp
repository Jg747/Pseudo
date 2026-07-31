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

    std::string assign_var;
    bool assign;
    bool begin_str;
    int i;
    std::vector<int> indexing_depth;
    ArrayValue* arr;
    int init_arr;

    void init();
    void index_op(std::stack<std::shared_ptr<Value>>& values);
    void assign_op(std::stack<std::shared_ptr<Value>>& values, VariableContext& context);
    void identifier_op(const Token& token, std::stack<std::shared_ptr<Value>>& values, VariableContext& context);
    void unaryminus_op(std::stack<std::shared_ptr<Value>>& values);
    void arraysize_op(std::stack<std::shared_ptr<Value>>& values);
    void init_copy_array(std::stack<std::shared_ptr<Value>>& values);
    void operation(std::stack<std::shared_ptr<Value>>& values, const Token& token) const;
    void assign_deep_index(ArrayValue* value, std::shared_ptr<Value>& val, std::vector<int> indexes, size_t depth);
};

#endif // __EXPRESSION_HPP__