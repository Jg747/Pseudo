#ifndef __FUNCTIONANALYZER_HPP__
#define __FUNCTIONANALYZER_HPP__

#include "analyzer.hpp"
#include "literals/function.hpp"

#include <unordered_map>

class FunctionAnalyzer : public Analyzer {
private:
    std::unordered_map<std::string, Function> funcs;
    std::string program_entry_point;

public:
    std::unordered_map<std::string, Function>& get_funcs() const;
};

#endif // __FUNCTIONANALYZER_HPP__