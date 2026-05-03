#ifndef __LOOP_HPP__
#define __LOOP_HPP__

#include "instruction.hpp"
#include "../condition.hpp"

#include <vector>

class Loop : public Instruction {
protected:
    std::vector<Instruction> instructions;
    Condition condition;

public:
    std::vector<Instruction>& get_instructions() const;
    void add_instruction(Instruction& i);
    void remove_instruction(Instruction& i);
    void remove_instruction(int idx);

    std::vector<Condition>& get_conditions() const;
    void set_condition(std::vector<Condition>& conditions);
    void add_condition(Condition condition);
    
    virtual bool test_condition() = 0;
};

#endif // __LOOP_HPP__