#ifndef __SYNTAXANALYZER_HPP__
#define __SYNTAXANALYZER_HPP__

#include "analyzer.hpp"
#include "lang.hpp"
#include "components/instructions/instruction.hpp"

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <regex>
#include <optional>
#include <memory>

class InstructionAnalyzer;

class SyntaxAnalyzer : public Analyzer {
private:
    static const char* whitespaces;
    static const std::regex var_start_regex;
    static const std::regex allowed_operators;
    static const std::regex expr_regex;
    static std::unordered_map<std::string, tokens_e> keywords;

    static void load_keywords();

    std::unique_ptr<InstructionAnalyzer> cur_instruction;
    bool var_flag = false;
    
    bool analyze_instruction(std::vector<std::string>* tokens, std::size_t* index);
    bool set_cur_instruction(std::optional<tokens_e>& token);

public:
    SyntaxAnalyzer(std::string filename);
    bool analyze() override;
    static std::optional<tokens_e> analyze_token(std::string& token);
    bool analyze_line(std::string line);
    static std::vector<std::string> tokenize_string(std::string string);
    static void trim_string(std::string& string);

    bool get_var_flag() const;
};



class InstructionAnalyzer {
protected:
    SyntaxAnalyzer* a;
    std::vector<std::string>* tokens;
    std::size_t* cur_index;
public:
    void set_params(SyntaxAnalyzer *a, std::vector<std::string>* tokens, std::size_t* index);
    virtual bool analyze_syntax() = 0;
};

class AssignationAnalyzer : public InstructionAnalyzer {
public:
    bool analyze_syntax() override;
};

class UntilAnalyzer : public InstructionAnalyzer {
private:
    enum class states_e { COND_START, EXPR, COND_END };
public:
    bool analyze_syntax() override;
};

/*class WhileAnalyzer : public InstructionAnalyzer {
public:
    bool analyze_syntax() override;
};

class ForAnalyzer : public InstructionAnalyzer {
public:
    bool analyze_syntax() override;
};

class IfAnalyzer : public InstructionAnalyzer {
public:
    bool analyze_syntax() override;
};

class ElifAnalyzer : public InstructionAnalyzer {
public:
    bool analyze_syntax() override;
};

class SwitchAnalyzer : public InstructionAnalyzer {
public:
    bool analyze_syntax() override;
};

class CaseAnalyzer : public InstructionAnalyzer {
public:
    bool analyze_syntax() override;
};

class ReadAnalyzer : public InstructionAnalyzer {
public:
    bool analyze_syntax() override;
};

class WriteAnalyzer : public InstructionAnalyzer {
public:
    bool analyze_syntax() override;
};

class FunctionSyntaxAnalyzer : public InstructionAnalyzer {
public:
    bool analyze_syntax() override;
};

class ReturnAnalyzer : public InstructionAnalyzer {
public:
    bool analyze_syntax() override;
};*/

#endif // __SYNTAXANALYZER_HPP__