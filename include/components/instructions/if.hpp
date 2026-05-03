#ifndef __IF_HPP__
#define __IF_HPP__

#include "instruction.hpp"
#include "../condition.hpp"

#include <vector>

struct sequence {
    Condition condition;
    std::vector<Instruction> instructions;

    bool test_condition();
};

class If : public Instruction {
private:
    std::vector<sequence> cases;

public:
    std::vector<sequence>& get_cases() const;
    void add_sequence(sequence& s);

    sequence get_sequence(int idx) const;
    std::size_t cases_count() const;
};

#endif // __IF_HPP__