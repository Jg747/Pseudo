/*
TODO
function support
function syntax support
parenthesis correctness check
shunting-yard
*/

#include "analyzers/syntaxanalyzer.hpp"
#include "lang.hpp"
#include "expr.hpp"

#include <fstream>
#include <string>
#include <vector>
#include <regex>

const char* SyntaxAnalyzer::whitespaces = " \n\t";
const std::regex SyntaxAnalyzer::var_start_regex(ALLOWED_VARSFUNCS_CHARS);
const std::regex SyntaxAnalyzer::allowed_operators(ALLOWED_OPERATORS);
const std::regex SyntaxAnalyzer::expr_regex(EXPR_CHARS);
std::unordered_map<std::string, tokens_e> SyntaxAnalyzer::keywords;


SyntaxAnalyzer::SyntaxAnalyzer(std::string filename) : Analyzer(filename) {
    load_keywords();
}

void SyntaxAnalyzer::load_keywords() {
    keywords.insert({ std::string(BEGIN_STR), tokens_e::BEGIN });
    keywords.insert({ std::string(END_STR), tokens_e::END });
    keywords.insert({ std::string(IF_STR), tokens_e::IF });
    keywords.insert({ std::string(THEN_STR), tokens_e::THEN });
    keywords.insert({ std::string(ELIF_STR), tokens_e::ELIF });
    keywords.insert({ std::string(ELSE_STR), tokens_e::ELSE });
    keywords.insert({ std::string(ENDIF_STR), tokens_e::ENDIF });
    keywords.insert({ std::string(WHILE_STR), tokens_e::WHILE });
    keywords.insert({ std::string(ENDWHILE_STR), tokens_e::ENDWHILE });
    keywords.insert({ std::string(SWITCH_STR), tokens_e::SWITCH });
    keywords.insert({ std::string(CASE_STR), tokens_e::CASE });
    keywords.insert({ std::string(CASEDEFAULT_STR), tokens_e::CASEDEFAULT });
    keywords.insert({ std::string(ENDSWITCH_STR), tokens_e::ENDSWITCH });
    keywords.insert({ std::string(REPEAT_STR), tokens_e::REPEAT });
    keywords.insert({ std::string(UNTIL_STR), tokens_e::UNTIL });
    keywords.insert({ std::string(READ_STR), tokens_e::READ });
    keywords.insert({ std::string(WRITE_STR), tokens_e::WRITE });
    keywords.insert({ std::string(ASSIGN_STR), tokens_e::ASSIGN });
    keywords.insert({ std::string(FUNCTION_STR), tokens_e::FUNCTION });
    keywords.insert({ std::string(RETURN_STR), tokens_e::RETURN });
}

bool SyntaxAnalyzer::analyze() {
    std::ifstream in(this->path);
    std::string str;

    while (std::getline(in, str)) {
        printf("%s\n", str.c_str());
        if (!analyze_line(str)) {
            in.close();
            return false;
        }
        cur_line++;
    }

    in.close();
    return true;
}

std::optional<tokens_e> SyntaxAnalyzer::analyze_token(std::string& token) {
    if (token.empty() || token.size() == 0) {
        return {};
    }

    if (!keywords.contains(token.c_str())) {
        if (std::regex_match(token, var_start_regex)) {
            return tokens_e::VAR;
        } else if (std::regex_match(token, allowed_operators)) {
            return tokens_e::OPERATOR;
        } else if (std::regex_match(token, expr_regex)) {
            return tokens_e::EXPR_TOK;
        }
        return {};
    }
    
    return keywords[token.c_str()];
}

bool SyntaxAnalyzer::get_var_flag() const {
    return var_flag;
}

bool SyntaxAnalyzer::analyze_instruction(std::vector<std::string>* tokens, std::size_t* index) {
    cur_instruction->set_params(this, tokens, index);
    return cur_instruction->analyze_syntax();
}

bool SyntaxAnalyzer::set_cur_instruction(std::optional<tokens_e>& token) {
    if (!token) {
        return false;
    }

    switch (token.value()) {
        case tokens_e::ASSIGN:
            cur_instruction = std::make_unique<AssignationAnalyzer>();
            break;
        case tokens_e::UNTIL:
            cur_instruction = std::make_unique<UntilAnalyzer>();
            break;
        /*case tokens_e::WHILE:
            cur_instruction = std::make_unique<WhileAnalyzer>();
            break;
        case tokens_e::IF:
            cur_instruction = std::make_unique<IfAnalyzer>();
            break;
        case tokens_e::ELIF:
            cur_instruction = std::make_unique<ElifAnalyzer>();
            break;
        case tokens_e::SWITCH:
            cur_instruction = std::make_unique<SwitchAnalyzer>();
            break;
        case tokens_e::CASE:
            cur_instruction = std::make_unique<CaseAnalyzer>();
            break;
        case tokens_e::READ:
            cur_instruction = std::make_unique<ReadAnalyzer>();
            break;
        case tokens_e::WRITE:
            cur_instruction = std::make_unique<WriteAnalyzer>();
            break;
        case tokens_e::FUNCTION:
            cur_instruction = std::make_unique<FunctionSyntaxAnalyzer>();
            break;
        case tokens_e::RETURN:
            cur_instruction = std::make_unique<ReturnAnalyzer>();
            break;*/
        default: return false;
    }
    return true;
}

bool SyntaxAnalyzer::analyze_line(std::string line) {
    std::vector<std::string> tokens = tokenize_string(line);
    std::size_t index = 0;

    while (index < tokens.size()) {
        std::string token = tokens[index];
        auto cur_tk = analyze_token(token);
        
        if (!cur_tk.has_value()) {
            stop_interpreter("Unknown token");
            return false;
        }

        if (cur_tk.value() == tokens_e::VAR) {
            if (var_flag) {
                stop_interpreter("Assign operation error: need an assignation operator combined with a variable");
                return false;
            }
            var_flag = true;
            index++;
            continue;
        }

        if (set_cur_instruction(cur_tk)) {
            if (var_flag && !dynamic_cast<AssignationAnalyzer*>(cur_instruction.get())) {
                stop_interpreter("Assign operation error: need an assignation operator combined with a variable");
                return false;
            }

            if (!analyze_instruction(&tokens, &index)) {
                return false;
            }
        } else {
            if (var_flag && !dynamic_cast<AssignationAnalyzer*>(cur_instruction.get())) {
                stop_interpreter("Assign operation error: need an assignation operator combined with a variable");
                return false;
            }
            index++;
        }

        var_flag = false;
    }
    return true;
}

std::vector<std::string> SyntaxAnalyzer::tokenize_string(std::string string) {
    std::istringstream stream(string);
    std::vector<std::string> ret;
    std::string token;
    while (std::getline(stream, token, ' ')) {
        trim_string(token);
        // TODO FUNCTION TOKENS
        if (token.size() > 0) {
            ret.push_back(token);
        }
    }
    return ret;
}

void SyntaxAnalyzer::trim_string(std::string& string) {
    std::size_t start = string.find_first_not_of(whitespaces);
    if (start == std::string::npos) {
        return;
    }

    std::size_t end = string.find_last_not_of(whitespaces);
    string = string.substr(start, end - start + 1);
}

void InstructionAnalyzer::set_params(SyntaxAnalyzer* a, std::vector<std::string>* tokens, std::size_t* index) {
    this->tokens = tokens;
    this->cur_index = index;
    this->a = a;
}



bool AssignationAnalyzer::analyze_syntax() {
    if (!a->get_var_flag()) {
        a->stop_interpreter("Assignation error (need a variable to assign to)");
        return false;
    }

    (*cur_index)++;
    std::size_t entry_index = *cur_index;
    
    std::vector<std::string> expression;
    
    while ((*cur_index) < tokens->size()) {
        auto opt = SyntaxAnalyzer::analyze_token(tokens->at(*cur_index));
        // TODO FUNCTION ASSIGNATION CASE
        if (opt.has_value()) {
            if (opt.value() == tokens_e::VAR || opt.value() == tokens_e::OPERATOR || opt.value() == tokens_e::EXPR_TOK || opt.value() == tokens_e::FUNCTION) {
                // ADD FUNCTION
                expression.push_back(tokens->at(*cur_index));
                (*cur_index)++;
            } else {
                return true;
            }
        } else {
            a->stop_interpreter("Assignation error (expression evaluation failed)");
            return false;
        }
    }

    if ((*cur_index) == entry_index) {
        a->stop_interpreter("Assignation error (need to assign a value)");
        return false;
    }

    if (!Expression::evaluate(expression)) {
        a->stop_interpreter("Assignation error (expression evaluation failed)");
        return false;
    }
    return true;
}

bool UntilAnalyzer::analyze_syntax() {
    UntilAnalyzer::states_e state = UntilAnalyzer::states_e::COND_START;
    std::vector<std::string> expression;

    (*cur_index)++;

    if (tokens->at(*cur_index) == COND_START_COND) {
        state = UntilAnalyzer::states_e::EXPR;
    }

    if (state == UntilAnalyzer::states_e::COND_START) {
        if (tokens->at(*cur_index).find(COND_START_COND) == 0) {
            state = UntilAnalyzer::states_e::EXPR;
            expression.push_back(tokens->at(*cur_index).substr(1));
        }
    }

    if (state != UntilAnalyzer::states_e::EXPR) {
        a->stop_interpreter("Missing condition start ('" + std::string(COND_START_COND) + "')");
        return false;
    }

    (*cur_index)++;

    while (*cur_index < tokens->size()) {
        std::string token = tokens->at(*cur_index);
        if (token.ends_with(COND_END_COND)) {
            if (token != COND_END_COND) {
                token = token.substr(0, token.find_last_of(COND_END_COND));
                expression.push_back(token);
            }

            if (!Expression::evaluate(expression)) {
                a->stop_interpreter("Condition evaluation failed");
                return false;
            }
            (*cur_index)++;
            return true;
        }
        expression.push_back(token);
        (*cur_index)++;
    }

    a->stop_interpreter("Missing condition end ('" + std::string(COND_END_COND) + "')");
    return false;
}