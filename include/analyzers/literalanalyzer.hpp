#ifndef __LITERALANALYZER_HPP__
#define __LITERALANALYZER_HPP__

#include "components/literals/variable.hpp"
#include "analyzers/analyzer.hpp"

#include <unordered_map>
#include <string>

class LiteralAnalyzer : public Analyzer {
private:
    std::unordered_map<std::string, Variable> vars;

public:
    std::unordered_map<std::string, Variable>& get_vars() const;
};

#endif // __LITERALANALYZER_HPP__