#include "interpreter/analyzers/programanalyzer.hpp"
#include "interpreter/analyzers/syntaxanalyzer.hpp"

#include <fstream>
#include <string>
#include <cstring>
#include <utility>

ProgramAnalyzer::ProgramAnalyzer(std::ifstream& in) : Analyzer(in, this->cur_line) {
    cur_line = 0;
}

std::unordered_map<std::string, std::unique_ptr<Function>>&& ProgramAnalyzer::get_funcs() {
    return std::move(funcs);
}

std::string ProgramAnalyzer::get_entry_point() {
    return program_entry_point;
}

std::vector<std::pair<std::string, Expression>>& ProgramAnalyzer::get_global_vars() {
    return global_vars;
}

void ProgramAnalyzer::add_global_variable(std::pair<std::string, Expression>&& var) {
    global_vars.push_back(var);
}

void ProgramAnalyzer::add_function(std::unique_ptr<Function>& f) {
    if (funcs.size() == 0) {
        program_entry_point = f->get_name();
    }
    funcs.emplace(f->get_name(), std::move(f));
}

bool ProgramAnalyzer::analyze() {
    std::string str;
    while (std::getline(in, str)) {
        cur_line++;
        if (!analyze_line(str)) {
            return false;
        }
    }
    return true;
}

bool ProgramAnalyzer::analyze_line(std::string str) {
    if (str.empty()) {
        return true;
    }

    _cur_line = str;
    cur_tokens = tokenize_string(str);
    cur_index = 0;
    return analyze_tokens();
}

bool ProgramAnalyzer::analyze_tokens() {
    while (cur_index < cur_tokens.size()) {
        auto token = cur_tokens[cur_index];
        auto cur_tk = analyze_token(token.first);

        if (!cur_tk.has_value()) {
            stop_interpreter("Unknown token (" + token.first + ")");
            return false;
        }

        switch (cur_tk.value()) {
            case tokens_e::FUNCTION:
                if (!analyze_function()) {
                    return false;
                }
                break;
            case tokens_e::VAR:
                if (!analyze_expr()) {
                    return false;
                }
                break;
            default:
                stop_interpreter("Invalid token '" + token.first + "' outside of function");
                return false;
        }
    }

    return true;
}

bool ProgramAnalyzer::analyze_expr() {
    GlobalAssignationAnalyzer a;
    a.set_params(this, &cur_tokens, &cur_index);
    return a.analyze_syntax();
}

bool ProgramAnalyzer::analyze_function() {
    FunctionAnalyzer f;
    f.set_params(this, &cur_tokens, &cur_index);
    return f.analyze_syntax();
}



void FragmentAnalyzer::set_params(ProgramAnalyzer *a, std::vector<std::pair<std::string, std::size_t>>* tokens, std::size_t* index) {
    this->a = a;
    this->tokens = tokens;
    this->cur_index = index;
}



bool FunctionAnalyzer::analyze_syntax() {
    (*cur_index)++;
    state = states_e::NAME;

    while (*cur_index < tokens->size()) {
        switch (state) {
            case states_e::NAME:
                if (!analyze_name()) {
                    return false;
                }
                break;
            case states_e::FIRST_PAREN: {
                if (!analyze_first_paren()) {
                    return false;
                }
                break;
            }
            case states_e::VARS:
                return analyze_args();
        }
    }

    a->stop_interpreter("Function syntax is '" + std::string(FUNCTION_STR) + " <name>(<arg1>, <arg2>, ...)'");
    return false;
}

bool FunctionAnalyzer::analyze_first_paren() {
    std::string& str = std::ref((*tokens)[*cur_index].first);

    if (str.find(FUNC_START_ARGS) == std::string::npos) {
        a->stop_interpreter("Function syntax is '" + std::string(FUNCTION_STR) + " <name>(<arg1>, <arg2>, ...)'");
        return false;
    }

    (*tokens)[*cur_index].second += str.find(FUNC_START_ARGS) + 1;
    str = str.substr(str.find(FUNC_START_ARGS) + 1);
    state = states_e::VARS;
    return true;
}

bool FunctionAnalyzer::analyze_name() {
    std::string name = (*tokens)[*cur_index].first;

    if (name.find(FUNC_START_ARGS) != std::string::npos && name.find(FUNC_START_ARGS) == name.rfind(FUNC_START_ARGS)) {
        state = states_e::VARS;
        name = name.substr(0, name.find(FUNC_START_ARGS));
        
        std::string& temp = std::ref((*tokens)[*cur_index].first);
        (*tokens)[*cur_index].second += temp.find(FUNC_START_ARGS) + 1;
        temp = temp.substr(temp.find(FUNC_START_ARGS) + 1);
        (*cur_index)--;
    } else {
        state = states_e::FIRST_PAREN;
    }

    if (!Variable::is_name_correct(name)) {
        a->stop_interpreter("'" + name + "' is not a valid function name");
        return false;
    }

    this->name = name;

    (*cur_index)++;
    return true;
}

bool FunctionAnalyzer::analyze_args() {
    init();

    skip_first_spaces();

    if (i >= line.size()) {
        a->stop_interpreter(FUNCTION_SYNTAX_NO_ARG);
        return false;
    }

    if (line[i] == FUNC_END_ARGS) {
        end = true;
        return create_function();
    }

    while (i < line.size()) {
        if (!first_arg) {
            if (!comma()) {
                return false;
            }
        }

        first_arg = false;

        if (!parse_expression()) {
            return false;
        }

        if (end) {
            if (line.find_last_not_of(Analyzer::whitespaces) == i) {
                return create_function();
            }
            return false;
        }
    }

    return true;
}

void FunctionAnalyzer::init() {
    line = a->get_cur_line().substr((*tokens)[*cur_index].second);

    i = 0;
    first_arg = true;
    space = false;
    end = false;
}

void FunctionAnalyzer::skip_first_spaces() {
    while (i < line.size() && std::isspace(line[i])) {
        if (!space && std::isspace(line[i])) {
            space = true;
            (*cur_index)++;
        }
        i++;
    }

    space = false;
}

void FunctionAnalyzer::skip_spaces() {
    for (i += 1; i < line.size() && std::isspace(line[i]); i++) {
        if (!space && std::isspace(line[i])) {
            space = true;
            (*cur_index)++;
        }
    }

    space = false;
}

bool FunctionAnalyzer::comma() {
    if (line[i] != READ_SEPARATOR) {
        a->stop_interpreter(FUNCTION_SYNTAX_ERROR);
        return false;
    }

    skip_spaces();

    if (i >= line.size()) {
        a->stop_interpreter(FUNCTION_SYNTAX_COMMA_ERROR);
        return false;
    }

    return true;
}

std::string FunctionAnalyzer::get_expression() {
    std::string var = "";
    for (; i < line.size() && line[i] != READ_SEPARATOR && line[i] != FUNC_END_ARGS; i++) {
        if (!space && std::isspace(line[i])) {
            (*cur_index)++;
            space = true;
        } else if (space && !std::isspace(line[i])) {
            space = false;
        }
        var += line[i];
    }
    SyntaxAnalyzer::trim_string(var);

    space = false;

    if (line[i] == FUNC_END_ARGS) {
        end = true;
    }

    return var;
}

bool FunctionAnalyzer::check_var_name(std::string& var) {
    if (!Variable::is_name_correct(var)) {
        a->stop_interpreter("'" + var + "' is not a valid variable name");
        return false;
    }
    return true;
}

bool FunctionAnalyzer::parse_expression() {
    std::string var = get_expression();

    if (var.empty()) {
        a->stop_interpreter(FUNCTION_SYNTAX_COMMA_ERROR);
        return false;
    }

    if (!check_var_name(var)) {
        return false;
    }

    args.push_back(var);
    return true;
}

bool FunctionAnalyzer::create_function() {
    (*cur_index)++;
    
    auto ptr = std::make_unique<Function>(name, args);

    SyntaxAnalyzer s(a);
    if (!s.analyze()) {
        return false;
    }

    ptr->set_vars(s.get_var_list());
    ptr->set_instructions(s.get_instructions());
    
    a->add_function(ptr);

    return true;
}



bool GlobalAssignationAnalyzer::analyze_syntax() {
    std::string line = a->get_cur_line().substr(tokens->at(*cur_index).second);

    l = line.substr(0, line.find(ASSIGN_STR));
    if (l.empty()) {
        a->stop_interpreter("Error on left side of expression (var to assign)");
        return false;
    }

    r = line.substr(line.find(ASSIGN_STR) + strlen(ASSIGN_STR));
    if (r.empty()) {
        a->stop_interpreter("Error on right side of expression");
        return false;
    }

    increment_index(l);
    increment_index(r);

    if (!create_instruction()) {
        return false;
    }

    return true;
}

void GlobalAssignationAnalyzer::increment_index(std::string& str) {
    bool space = false;
    for (std::size_t i = 0; i < str.length(); i++) {
        if (!space && std::isspace(str[i])) {
            space = true;
            (*cur_index)++;
        } else if (space && !std::isspace(str[i])) {
            space = false;
        }
    }
}

bool GlobalAssignationAnalyzer::create_instruction() {
    (*cur_index)++;

    try {
        auto l_vars = Expression::parse_expression(l).req_vars();
        if (l_vars.size() == 0) {
            a->stop_interpreter("Error on left side of expression (var to assign)");
            return false;
        }

        Expression expr = Expression::parse_expression(l + " = " + r);

        // la prima variabile e' sempre quella a cui viene assegnato il valore
        // le altre sono operatori usati in caso sia una cella di un array
        a->add_global_variable({ l_vars[0], expr });
    } catch (const std::runtime_error& e) {
        a->stop_interpreter("Assignation error: " + std::string(e.what()));
        return false;
    }
    return true;
}