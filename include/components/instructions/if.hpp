#ifndef __IF_HPP__
#define __IF_HPP__

#include "scopedinstruction.hpp"
#include "instruction.hpp"
#include "expression/expression.hpp"

#include <list>
#include <vector>

struct sequence {
    Expression condition;
    std::list<Instruction> instructions;

    bool test_condition();
};

class If : public ScopedInstruction {
private:
    std::vector<sequence> cases;

    void abstract() override {}

public:
    std::vector<sequence>& get_cases() const;
    void add_sequence(sequence& s);

    sequence get_sequence(int idx) const;
    std::size_t cases_count() const;
};

#endif // __IF_HPP__