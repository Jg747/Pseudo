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

typedef struct {
    std::string lit;
    bool is_variable;
} write_literal;

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
    std::string _cur_line;
    std::stack<char> parenthesis;
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
    bool eof();

    bool get_var_flag() const;
    bool end_tokens() const;
    std::string get_cur_line();
    void pop_top();
    void pop_next();
};



class InstructionAnalyzer {
protected:
    SyntaxAnalyzer* a;
    std::vector<std::string>* tokens;
    std::size_t* cur_index;
    bool begin;
public:
    void set_params(SyntaxAnalyzer *a, std::vector<std::string>* tokens, std::size_t* index);
    void set_begin();
    static bool get_condition(SyntaxAnalyzer* a, std::size_t* cur_index, std::vector<std::string>* tokens, std::vector<std::string>& expression);
    virtual bool analyze_syntax() = 0;
    virtual void init_state();
    virtual bool next_state(tokens_e token);
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
    bool next_state(tokens_e token) override;
};

class WhileAnalyzer : public InstructionAnalyzer {
private:
    enum class states_e { COND_START, EXPR, COND_END, BODY_BEGIN, BODY_END };
    WhileAnalyzer::states_e state;
public:
    bool analyze_syntax() override;
    void init_state() override;
    bool next_state(tokens_e token) override;
};

class IfAnalyzer : public InstructionAnalyzer { // IF, ELSE, ELIF
private:
    enum class states_e { COND_START, EXPR, THEN, BODY, ELSE, ELIF, ENDIF };
    IfAnalyzer::states_e state;
    char section_type;
    bool analyze_condition();
public:
    void set_section(char section);
    bool analyze_syntax() override;
    void init_state() override;
    bool next_state(tokens_e token) override;
};

class WriteAnalyzer : public InstructionAnalyzer {
private:
    // aggiungere una union per capire se è una var o un literal e fare merge dei vector per eseguire la stampa senza mantenere separati
    std::vector<write_literal> literals;
public:
    bool analyze_syntax() override;
    void init_state() override;
    bool next_state(tokens_e token) override;
};

class ReadAnalyzer : public InstructionAnalyzer {
private:
    std::vector<std::string> vars;
public:
    bool analyze_syntax() override;
    void init_state() override;
    bool next_state(tokens_e token) override;
};

/*class FunctionSyntaxAnalyzer : public InstructionAnalyzer {
public:
    bool analyze_syntax() override;
};*/

#endif // __SYNTAXANALYZER_HPP__