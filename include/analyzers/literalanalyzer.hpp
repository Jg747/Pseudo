#ifndef __LITERALANALYZER_HPP__
#define __LITERALANALYZER_HPP__

#include "analyzer.hpp"
#include "literals/literal.hpp"

#include <unordered_map>

class LiteralAnalyzer : public Analyzer {
private:
    std::unordered_map<std::string, Literal> vars;

public:
    std::unordered_map<std::string, Literal>& get_vars() const;
};

#endif // __LITERALANALYZER_HPP__