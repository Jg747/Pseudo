#ifndef __LITERALANALYZER_HPP__
#define __LITERALANALYZER_HPP__

#include "components/literals/literal.hpp"
#include "analyzers/analyzer.hpp"
#include "analyzers/literalanalyzer.hpp"

#include <unordered_map>
#include <string>

class LiteralAnalyzer : public Analyzer {
private:
    std::unordered_map<std::string, Literal> vars;

public:
    std::unordered_map<std::string, Literal>& get_vars() const;
};

#endif // __LITERALANALYZER_HPP__