#ifndef __WRITE_HPP__
#define __WRITE_HPP__

#include "components/instructions/instruction.hpp"
#include "expression/variablecontext.hpp"
#include "expression/expression.hpp"

#include <list>
#include <string>

class WriteLiteral {
public:
    virtual std::string print(VariableContext& vars) = 0;
};

class WriteExpr : public WriteLiteral {
private:
    Expression e;
public:
    WriteExpr(Expression&& e) : e(e) {}
    std::string print(VariableContext& vars) override;
};

class WriteString : public WriteLiteral {
private:
    std::string s;
public:
    WriteString(std::string& s) : s(s) {}
    std::string print(VariableContext& vars) override { return s; }
};

class Write : public Instruction {
private:
    std::list<std::shared_ptr<WriteLiteral>> print_order;
public:
    Write(std::list<std::shared_ptr<WriteLiteral>>& print_order) : print_order(print_order) {}
    void execute(VariableContext& scoped_vars) override;
};

#endif // __WRITE_HPP__