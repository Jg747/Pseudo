#ifndef __SCOPED_INSTRUCTION_HPP__
#define __SCOPED_INSTRUCTION_HPP__

#include "instruction.hpp"

#include <list>
#include <memory>

class ScopedInstruction : public Instruction {
protected:
    std::list<std::unique_ptr<Instruction>> instructions;

    virtual void abstract() = 0;

public:
    ScopedInstruction() {}
    std::list<std::unique_ptr<Instruction>>& get_instructions();
    virtual void add_instruction(std::unique_ptr<Instruction>& i);
};

#endif /* __SCOPED_INSTRUCTION_HPP__ */