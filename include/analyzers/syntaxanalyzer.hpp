#ifndef __SYNTAXANALYZER_HPP__
#define __SYNTAXANALYZER_HPP__

#include "analyzer.hpp"

#include <string>

class SyntaxAnalyzer : public Analyzer {
public:
    bool analyze_token(std::string token);
    bool analyze_line(std::string line);
};

#endif // __SYNTAXANALYZER_HPP__