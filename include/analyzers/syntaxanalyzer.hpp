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
#include <fstream>
#include <stack>

class InstructionAnalyzer;

class SyntaxAnalyzer : public Analyzer {
private:
    enum add_instruction_ret { NO_TOKEN, ERROR, NEXT, CALLBACK };

    static const char* whitespaces;
    static const std::regex var_start_regex;
    static const std::regex allowed_operators;
    static const std::regex expr_regex;
    static std::unordered_map<std::string, tokens_e> keywords;
    
    static void load_keywords();

    std::ifstream in;
    std::vector<std::string> cur_tokens;
    std::size_t cur_index;
    bool var_flag = false;
    bool pop_next_flag = false;

    std::stack<std::unique_ptr<InstructionAnalyzer>> cur_instruction;
    
    bool analyze_instruction();
    bool prev_instr_is_assign();
    add_instruction_ret add_cur_instruction(std::optional<tokens_e>& token);

    template<class T>
    T* get_cur_instruction_top_ptr();

public:
    static std::optional<tokens_e> analyze_token(std::string& token);
    static std::vector<std::string> tokenize_string(std::string string);
    static void trim_string(std::string& string);
    static bool is_keyword(std::string& token);
    
    template<class T, class R>
    static bool is_a(std::unique_ptr<T>& ptr);
    
    SyntaxAnalyzer(std::string filename);

    bool analyze() override;
    int analyze_line(std::string line);
    bool get_next_line();
    int analyze_tokens();

    bool get_var_flag() const;
    bool end_tokens() const;
    void pop_next();
};



class InstructionAnalyzer {
protected:
    SyntaxAnalyzer* a;
    std::vector<std::string>* tokens;
    std::size_t* cur_index;
public:
    void set_params(SyntaxAnalyzer *a, std::vector<std::string>* tokens, std::size_t* index);
    virtual bool analyze_syntax() = 0;
    virtual void init_state();
    virtual void next_state();
};

class AssignationAnalyzer : public InstructionAnalyzer {
public:
    bool analyze_syntax() override;
};

class UntilAnalyzer : public InstructionAnalyzer {
private:
    enum class states_e { BODY, UNTIL, COND_START, EXPR, COND_END };
    UntilAnalyzer::states_e state;
public:
    bool analyze_syntax() override;
    void init_state() override;
    void next_state() override;
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