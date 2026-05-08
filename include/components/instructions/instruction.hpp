#ifndef __INSTRUCTION_HPP__
#define __INSTRUCTION_HPP__

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
protected:
    virtual void execute() = 0;
};

#endif // __INSTRUCTION_HPP__