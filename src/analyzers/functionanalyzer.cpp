#ifndef __FUNCTIONANALYZER_HPP__
#define __FUNCTIONANALYZER_HPP__

#include "components/function.hpp"
#include "analyzers/analyzer.hpp"
#include "analyzers/functionanalyzer.hpp"

#include <unordered_map>
#include <string>

class FunctionAnalyzer : public Analyzer {
private:
    std::unordered_map<std::string, Function> funcs;
    std::string program_entry_point;

public:
    std::unordered_map<std::string, Function>& get_funcs() const;
};

#endif // __FUNCTIONANALYZER_HPP__