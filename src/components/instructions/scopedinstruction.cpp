#include "components/instructions/scopedinstruction.hpp"

#include <list>
#include <memory>

std::list<std::unique_ptr<Instruction>>& ScopedInstruction::get_instructions() {
    return this->instructions;
}

void ScopedInstruction::add_instruction(std::unique_ptr<Instruction>& i) {
    this->instructions.push_back(std::move(i));
}