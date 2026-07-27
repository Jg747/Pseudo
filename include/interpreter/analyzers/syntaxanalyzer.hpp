#ifndef __SYNTAXANALYZER_HPP__
#define __SYNTAXANALYZER_HPP__

#include "interpreter/analyzers/analyzer.hpp"
#include "lang.hpp"
#include "expression/expression.hpp"
#include "components/instructions/instruction.hpp"

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <regex>
#include <optional>
#include <memory>
#include <fstream>
#include <utility>
#include <expected>
#include <stack>
#include <list>

typedef struct {
    std::string lit;
    bool is_variable;
} write_literal;

class InstructionAnalyzer;

class SyntaxAnalyzer : public Analyzer {
private:
    std::list<std::unique_ptr<Instruction>> instructions;
    std::vector<std::string> var_list;

    enum add_instruction_ret { NO_TOKEN, ERROR, NEXT, CALLBACK };

    static const std::regex var_regex;
    static std::unordered_map<tokens_e, std::regex> keywords;
    
    static void load_keywords();

    std::ifstream in;
    std::vector<std::pair<std::string, std::size_t>> cur_tokens;
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
    static const char* whitespaces;
    
    static std::optional<tokens_e> analyze_token(std::string& token);
    static std::vector<std::pair<std::string, std::size_t>> tokenize_string(std::string string);
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

    std::list<std::unique_ptr<Instruction>>& get_instructions();
    std::vector<std::string>& get_var_list();
};



class InstructionAnalyzer {
protected:
    SyntaxAnalyzer* a;
    std::vector<std::pair<std::string, std::size_t>>* tokens;
    std::size_t* cur_index;
    bool begin;
    
    virtual bool create_instruction() = 0;
    std::expected<Expression, bool> get_condition();
public:
    void set_params(SyntaxAnalyzer *a, std::vector<std::pair<std::string, std::size_t>>* tokens, std::size_t* index);
    void set_begin();
    virtual bool analyze_syntax() = 0;
    virtual void init_state();
    virtual bool next_state(tokens_e token);
};

class AssignationAnalyzer : public InstructionAnalyzer {
private:
    std::string l;
    std::string r;

    bool create_instruction() override;

public:
    bool analyze_syntax() override;
};

class UntilAnalyzer : public InstructionAnalyzer {
private:
    enum class states_e { BODY, UNTIL };
    UntilAnalyzer::states_e state;

    bool create_instruction() override;

public:
    bool analyze_syntax() override;
    void init_state() override;
    bool next_state(tokens_e token) override;
};

class WhileAnalyzer : public InstructionAnalyzer {
private:
    enum class states_e { BODY_BEGIN, WAIT_END, BODY_END };
    WhileAnalyzer::states_e state;

    bool create_instruction() override;

public:
    bool analyze_syntax() override;
    void init_state() override;
    bool next_state(tokens_e token) override;
};

class IfAnalyzer : public InstructionAnalyzer { // IF, ELSE, ELIF
private:
    enum class states_e { COND_START, THEN, BODY, ELSE, ELIF, ENDIF };
    IfAnalyzer::states_e state;
    char section_type;
    
    bool analyze_condition();
    bool create_instruction() override;

public:
    void set_section(char section);
    bool analyze_syntax() override;
    void init_state() override;
    bool next_state(tokens_e token) override;
};

class WriteAnalyzer : public InstructionAnalyzer {
private:
    std::vector<write_literal> literals;

    bool create_instruction() override;

public:
    bool analyze_syntax() override;
    void init_state() override;
    bool next_state(tokens_e token) override;
};

class ReadAnalyzer : public InstructionAnalyzer {
private:
    std::vector<std::string> vars;

    bool create_instruction() override;

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