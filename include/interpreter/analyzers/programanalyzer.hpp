#ifndef __PROGRAMANALYZER_HPP__
#define __PROGRAMANALYZER_HPP__

#include "analyzer.hpp"
#include "components/literals/function.hpp"
#include "expression/expression.hpp"

#include <unordered_map>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <utility>

#define FUNCTION_SYNTAX_NO_ARG "FUNCTION_SYNTAX_NO_ARG"
#define FUNCTION_SYNTAX_ERROR "FUNCTION_SYNTAX_ERROR"
#define FUNCTION_SYNTAX_COMMA_ERROR "FUNCTION_SYNTAX_COMMA_ERROR"

class ProgramAnalyzer : public Analyzer {
private:
    std::vector<std::pair<std::string, Expression>> global_vars;
    std::unordered_map<std::string, std::unique_ptr<Function>> funcs;
    std::string program_entry_point;

    std::size_t cur_line;
    std::vector<std::pair<std::string, std::size_t>> cur_tokens;
    std::size_t cur_index;

    bool analyze_line(std::string str);
    bool analyze_tokens();

    bool analyze_expr();
    bool analyze_function();

public:
    ProgramAnalyzer(std::ifstream& in);
    std::unordered_map<std::string, std::unique_ptr<Function>>&& get_funcs();
    std::string get_entry_point();
    std::vector<std::pair<std::string, Expression>>& get_global_vars();

    bool analyze() override;
    void add_global_variable(std::pair<std::string, Expression>&& var);
    void add_function(std::unique_ptr<Function>& f);
};



class FragmentAnalyzer {
protected:
    ProgramAnalyzer* a;
    std::vector<std::pair<std::string, std::size_t>>* tokens;
    std::size_t* cur_index;

public:
    void set_params(ProgramAnalyzer *a, std::vector<std::pair<std::string, std::size_t>>* tokens, std::size_t* index);
    virtual bool analyze_syntax() = 0;
};

class GlobalAssignationAnalyzer : public FragmentAnalyzer {
private:
    std::string l;
    std::string r;

    bool create_instruction();
    void increment_index(std::string& str);

public:
    bool analyze_syntax() override;
};

class FunctionAnalyzer : public FragmentAnalyzer {
private:
    std::vector<std::string> args;
    std::string name;

    enum class states_e { NAME, FIRST_PAREN, VARS };
    states_e state;
    
    std::size_t i;
    std::string line;
    bool first_arg;
    bool space;
    bool end;

    bool analyze_name();
    bool analyze_first_paren();
    bool analyze_args();

    void init();
    void skip_first_spaces();
    void skip_spaces();
    bool comma();
    std::string get_expression();
    bool check_var_name(std::string& var);
    bool parse_expression();

    bool create_function();

public:
    bool analyze_syntax() override;
};

#endif // __PROGRAMANALYZER_HPP__