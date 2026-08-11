#ifndef __INSTRUCTION_HPP__
#define __INSTRUCTION_HPP__

#include "expression/variablecontext.hpp"

enum class instruction_e {
    ASSIGNATION,
    UNTIL,
    WHILE,
    FOR,
    IF,
    ELIF,
    SWITCH,
    CASE,
    READ,
    WRITE,
    FUNCTION,
    RETURN
};

class Instruction {
public:
    virtual bool execute(VariableContext& scoped_vars) = 0;
};

#endif // __INSTRUCTION_HPP__