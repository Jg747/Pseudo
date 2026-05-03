#ifndef __INSTRUCTION_HPP__
#define __INSTRUCTION_HPP__

class Instruction {
protected:
    virtual void execute() = 0;
};

#endif // __INSTRUCTION_HPP__