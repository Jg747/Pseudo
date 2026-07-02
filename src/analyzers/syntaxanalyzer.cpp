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
#include <memory>

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
    keywords.insert({ std::string(REPEAT_STR), tokens_e::REPEAT });
    keywords.insert({ std::string(UNTIL_STR), tokens_e::UNTIL });
    keywords.insert({ std::string(READ_STR), tokens_e::READ });
    keywords.insert({ std::string(WRITE_STR), tokens_e::WRITE });
    keywords.insert({ std::string(ASSIGN_STR), tokens_e::ASSIGN });
    keywords.insert({ std::string(FUNCTION_STR), tokens_e::FUNCTION });
    keywords.insert({ std::string(RETURN_STR), tokens_e::RETURN });
}

bool SyntaxAnalyzer::analyze() {
    in.open(this->path);

    cur_line = 0;
    if (!get_next_line()) {
        return false;
    }

    if (parenthesis.size() > 0) {
        stop_interpreter("missing " + std::string(END_STR) + " to end block(s)");
        return false;
    }

    in.close();
    return true;
}

bool SyntaxAnalyzer::get_next_line() {
    std::string str;
    while (std::getline(in, str)) {
        printf("%s\n", str.c_str());
        cur_line++;
        int ret = analyze_line(str);
        if (ret == 0) {
            in.close();
            return false;
        } else if (ret < 0) {
            return true;
        }
    }
    return true;
}

int SyntaxAnalyzer::analyze_line(std::string line) {
    _cur_line = line;
    cur_tokens = tokenize_string(line);
    cur_index = 0;
    return analyze_tokens();
}

int SyntaxAnalyzer::analyze_tokens() {
    while (cur_index < cur_tokens.size()) {
        std::string token = cur_tokens[cur_index];
        auto cur_tk = analyze_token(token);

        if (!cur_tk.has_value()) {
            stop_interpreter("Unknown token (" + token + ")");
            return 0;
        }

        if (cur_tk.value() == tokens_e::VAR) {
            if (var_flag) {
                stop_interpreter("Assign operation error: need an assignation operator combined with a variable");
                return 0;
            }
            var_flag = true;
            cur_index++;
            continue;
        }

        SyntaxAnalyzer::add_instruction_ret ret = add_cur_instruction(cur_tk);
        AssignationAnalyzer* assign = nullptr;

        switch (ret) {
            case SyntaxAnalyzer::add_instruction_ret::NO_TOKEN:
                if (var_flag && (cur_instruction.empty() || !dynamic_cast<AssignationAnalyzer*>(cur_instruction.top().get()))) {
                    stop_interpreter("Assign operation error: need an assignation operator combined with a variable");
                    return 0;
                }
                cur_index++;
                break;
            case SyntaxAnalyzer::add_instruction_ret::ERROR:
                return 0;
            case SyntaxAnalyzer::add_instruction_ret::NEXT:
                assign = get_cur_instruction_top_ptr<AssignationAnalyzer>();
                if (var_flag && !assign) {
                    stop_interpreter("Assign operation error: need an assignation operator combined with a variable");
                    return 0;
                }

                cur_index++;
                if (!analyze_instruction()) {
                    return 0;
                }
                
                if (!assign || pop_next_flag) {
                    if (pop_next_flag) {
                        if (cur_instruction.size() > 0) {
                            cur_instruction.pop();
                            // TODO remove IF when functions done
                        } else {
                            stop_interpreter("block ended without a start");
                        }
                    }
                    pop_next_flag = false;
                }
                break;
            case SyntaxAnalyzer::add_instruction_ret::CALLBACK:
                cur_index++;
                return -1;
            default:
                return 0;
        }

        var_flag = false;
    }
    return 1;
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

bool SyntaxAnalyzer::end_tokens() const {
    return cur_index >= cur_tokens.size();
}

bool SyntaxAnalyzer::prev_instr_is_assign() {
    return !cur_instruction.empty() && dynamic_cast<AssignationAnalyzer*>(cur_instruction.top().get());
}

void SyntaxAnalyzer::pop_next() {
    pop_next_flag = true;
}

void SyntaxAnalyzer::pop_top() {
    if (!cur_instruction.empty()) {
        cur_instruction.pop();
    }
}

template<class T>
T* SyntaxAnalyzer::get_cur_instruction_top_ptr() {
    if (cur_instruction.empty()) {
        return nullptr;
    }

    return dynamic_cast<T*>(cur_instruction.top().get());
}

template<class T, class R>
bool SyntaxAnalyzer::is_a(std::unique_ptr<T>& ptr) {
    return dynamic_cast<R*>(ptr);
}

bool SyntaxAnalyzer::analyze_instruction() {
    if (cur_instruction.size() == 0) {
        return true; // TODO modify when function are set
    }
    cur_instruction.top()->set_params(this, &cur_tokens, &cur_index);
    return cur_instruction.top()->analyze_syntax();
}

SyntaxAnalyzer::add_instruction_ret SyntaxAnalyzer::add_cur_instruction(std::optional<tokens_e>& token) {
    if (!token) {
        return SyntaxAnalyzer::add_instruction_ret::NO_TOKEN;
    }

    InstructionAnalyzer* a;

    switch (token.value()) {
        case tokens_e::BEGIN:
            parenthesis.push('(');
            if (cur_instruction.size() > 0) {
                a = cur_instruction.top().get();
                a->set_begin();
                a->next_state(token.value());
                return SyntaxAnalyzer::add_instruction_ret::CALLBACK;
            }
            break;
        case tokens_e::END:
            if (parenthesis.size() > 0) {
                parenthesis.pop();
            } else {
                stop_interpreter("missing " + std::string(BEGIN_STR) + " to begin block");
                return SyntaxAnalyzer::add_instruction_ret::ERROR;
            }

            if (cur_instruction.size() > 0) {
                a = cur_instruction.top().get();
                a->next_state(token.value());
                return SyntaxAnalyzer::add_instruction_ret::CALLBACK;
            }
            return SyntaxAnalyzer::add_instruction_ret::NO_TOKEN;
        case tokens_e::ASSIGN:
            if (prev_instr_is_assign()) {
                stop_interpreter("Unexpected other assignment token");
                return SyntaxAnalyzer::add_instruction_ret::ERROR;
            }
            cur_instruction.push(std::make_unique<AssignationAnalyzer>());
            break;
        case tokens_e::REPEAT:
            if (prev_instr_is_assign()) {
                stop_interpreter("No assignation provided");
                return SyntaxAnalyzer::add_instruction_ret::ERROR;
            }
            cur_instruction.push(std::make_unique<UntilAnalyzer>());
            break;
        case tokens_e::UNTIL:
            a = get_cur_instruction_top_ptr<UntilAnalyzer>();
            if (!a) {
                if (prev_instr_is_assign()) {
                    stop_interpreter("No assignation provided");
                } else {
                    stop_interpreter(std::string(UNTIL_STR) + " token without starting " + std::string(REPEAT_STR) + " found");
                }
                return SyntaxAnalyzer::add_instruction_ret::ERROR;
            }
            ((UntilAnalyzer*) a)->next_state(token.value());
            return SyntaxAnalyzer::add_instruction_ret::CALLBACK;
        case tokens_e::WHILE:
            cur_instruction.push(std::make_unique<WhileAnalyzer>());
            break;
        case tokens_e::IF:
            cur_instruction.push(std::make_unique<IfAnalyzer>());
            break;
        case tokens_e::THEN:
            a = get_cur_instruction_top_ptr<IfAnalyzer>();
            if (!a) {
                return SyntaxAnalyzer::add_instruction_ret::ERROR;
            }
            if (!((IfAnalyzer*) a)->next_state(token.value())) {
                return SyntaxAnalyzer::add_instruction_ret::ERROR;
            }
            return SyntaxAnalyzer::add_instruction_ret::CALLBACK;
        case tokens_e::ELSE:
            a = get_cur_instruction_top_ptr<IfAnalyzer>();
            if (!a) {
                return SyntaxAnalyzer::add_instruction_ret::ERROR;
            }
            ((IfAnalyzer*) a)->set_section(0);
            if (!((IfAnalyzer*) a)->next_state(token.value())) {
                return SyntaxAnalyzer::add_instruction_ret::ERROR;
            }
            return SyntaxAnalyzer::add_instruction_ret::CALLBACK;
        case tokens_e::ELIF:
            a = get_cur_instruction_top_ptr<IfAnalyzer>();
            if (!a) {
                return SyntaxAnalyzer::add_instruction_ret::ERROR;
            }
            ((IfAnalyzer*) a)->set_section(1);
            if (!((IfAnalyzer*) a)->next_state(token.value())) {
                return SyntaxAnalyzer::add_instruction_ret::ERROR;
            }
            return SyntaxAnalyzer::add_instruction_ret::CALLBACK;
        case tokens_e::ENDIF:
            a = get_cur_instruction_top_ptr<IfAnalyzer>();
            if (!a) {
                return SyntaxAnalyzer::add_instruction_ret::ERROR;
            }
            ((IfAnalyzer*) a)->set_section(-1);
            if (!((IfAnalyzer*) a)->next_state(token.value())) {
                return SyntaxAnalyzer::add_instruction_ret::ERROR;
            }
            return SyntaxAnalyzer::add_instruction_ret::CALLBACK;
        case tokens_e::WRITE:
            cur_instruction.push(std::make_unique<WriteAnalyzer>());
            break;
        case tokens_e::READ:
            cur_instruction.push(std::make_unique<ReadAnalyzer>());
            break;
        /*case tokens_e::FUNCTION:
            cur_instruction = std::make_unique<FunctionSyntaxAnalyzer>();
            break;
        case tokens_e::RETURN:
            cur_instruction = std::make_unique<ReturnAnalyzer>();
            break;*/
        default: return SyntaxAnalyzer::add_instruction_ret::NO_TOKEN;
    }
    return SyntaxAnalyzer::add_instruction_ret::NEXT;
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

bool SyntaxAnalyzer::is_keyword(std::string& token) {
    for (auto [k, v] : keywords) {
        if (k == token) {
            return true;
        }
    }
    return false;
}

bool SyntaxAnalyzer::eof() {
    return in.eof();
}

std::string SyntaxAnalyzer::get_cur_line() {
    return _cur_line;
}


void InstructionAnalyzer::set_params(SyntaxAnalyzer* a, std::vector<std::string>* tokens, std::size_t* index) {
    this->tokens = tokens;
    this->cur_index = index;
    this->a = a;
}

bool InstructionAnalyzer::next_state(tokens_e token) { return true; }

void InstructionAnalyzer::init_state() {}

void InstructionAnalyzer::set_begin() {
    begin = true;
}

bool InstructionAnalyzer::get_condition(SyntaxAnalyzer* a, std::size_t* cur_index, std::vector<std::string>* tokens, std::vector<std::string>& expression) {
    (*cur_index)++;

    while (*cur_index < tokens->size()) {
        std::string token = tokens->at(*cur_index);
        if (token.ends_with(COND_END_COND)) {
            if (token != std::string(1, COND_END_COND)) {
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

    a->stop_interpreter("Missing condition end ('" + std::string(1, COND_END_COND) + "')");
    return false;
}


bool AssignationAnalyzer::analyze_syntax() {
    if (!a->get_var_flag()) {
        a->stop_interpreter("Assignation error (need a variable to assign to)");
        return false;
    }

    std::size_t entry_index = *cur_index;
    
    std::vector<std::string> expression;
    bool valid = false;

    while ((*cur_index) < tokens->size()) {
        auto opt = SyntaxAnalyzer::analyze_token(tokens->at(*cur_index));
        // TODO FUNCTION ASSIGNATION CASE
        if (opt.has_value()) {
            if (opt.value() == tokens_e::VAR || opt.value() == tokens_e::OPERATOR || opt.value() == tokens_e::EXPR_TOK || opt.value() == tokens_e::FUNCTION) {
                // ADD FUNCTION
                valid = true;
                expression.push_back(tokens->at(*cur_index));
                (*cur_index)++;
            } else {
                if (!valid) {
                    a->stop_interpreter("Assignation error (no value assigned)");
                    return false;
                }
                break;
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
    a->pop_next();
    return true;
}


bool UntilAnalyzer::analyze_syntax() {
    init_state();
    if (!a->analyze_tokens()) {
        return false;
    }

    if (a->end_tokens()) {
        if (!a->get_next_line()) {
            return false;
        }
    }

    if (state != UntilAnalyzer::states_e::UNTIL) {
        a->stop_interpreter("No " + std::string(UNTIL_STR) + " keyword found");
        return false;
    }
    next_state(tokens_e::NONE);

    std::vector<std::string> expression;

    if (tokens->at(*cur_index)[0] == COND_START_COND) {
        next_state(tokens_e::NONE);
    }

    if (state == UntilAnalyzer::states_e::COND_START) {
        if (tokens->at(*cur_index).find(COND_START_COND) == 0) {
            next_state(tokens_e::NONE);
            expression.push_back(tokens->at(*cur_index).substr(1));
        }
    }

    if (state != UntilAnalyzer::states_e::EXPR) {
        a->stop_interpreter("Missing condition start ('" + std::string(1, COND_START_COND) + "')");
        return false;
    }

    return InstructionAnalyzer::get_condition(a, cur_index, tokens, expression);
}

void UntilAnalyzer::init_state() {
    state = UntilAnalyzer::states_e::BODY;
}

bool UntilAnalyzer::next_state(tokens_e token) {
    switch (state) {
        case UntilAnalyzer::states_e::BODY:
            state = UntilAnalyzer::states_e::UNTIL;
            break;
        case UntilAnalyzer::states_e::UNTIL:
            state = UntilAnalyzer::states_e::COND_START;
            break;
        case UntilAnalyzer::states_e::COND_START:
            state = UntilAnalyzer::states_e::EXPR;
            break;
        case UntilAnalyzer::states_e::EXPR:
            state = UntilAnalyzer::states_e::COND_END;
            break;
        default:
            break;
    }
    return true;
}


bool WhileAnalyzer::analyze_syntax() {
    init_state();
    std::vector<std::string> expression;

    if (tokens->at(*cur_index)[0] == COND_START_COND) {
        next_state(tokens_e::NONE);
    }

    if (state == WhileAnalyzer::states_e::COND_START) {
        if (tokens->at(*cur_index).find(COND_START_COND) == 0) {
            next_state(tokens_e::NONE);
            expression.push_back(tokens->at(*cur_index).substr(1));
        }
    }

    if (state != WhileAnalyzer::states_e::EXPR) {
        a->stop_interpreter("Missing condition start ('" + std::string(1, COND_START_COND) + "')");
        return false;
    }

    if (!InstructionAnalyzer::get_condition(a, cur_index, tokens, expression)) {
        return false;
    }
    next_state(tokens_e::NONE);

    while (!a->end_tokens()) {
        if (!a->analyze_tokens()) {
            return false;
        }
    }

    if (begin && state == WhileAnalyzer::states_e::BODY_END) {
        return true;
    }

    if (!a->get_next_line()) {
        return false;
    }

    if (begin && state == WhileAnalyzer::states_e::BODY_END) {
        return true;
    }

    if (!a->end_tokens()) {
        if (!a->analyze_tokens()) {
            return false;
        }
        if (begin && state == WhileAnalyzer::states_e::BODY_END) {
            return true;
        }
    }

    if (!a->get_next_line()) {
        return false;
    }

    if (begin && state == WhileAnalyzer::states_e::BODY_END) {
        return true;
    }

    a->stop_interpreter("need " + std::string(BEGIN_STR) + " after " + std::string(WHILE_STR) + " statement");
    return false;
}

void WhileAnalyzer::init_state() {
    state = WhileAnalyzer::states_e::COND_START;
}

bool WhileAnalyzer::next_state(tokens_e token) {
    switch (state) {
        case WhileAnalyzer::states_e::COND_START:
            state = WhileAnalyzer::states_e::EXPR;
            break;
        case WhileAnalyzer::states_e::EXPR:
            state = WhileAnalyzer::states_e::COND_END;
            break;
        case WhileAnalyzer::states_e::COND_END:
            state = WhileAnalyzer::states_e::BODY_BEGIN;
            break;
        case WhileAnalyzer::states_e::BODY_BEGIN:
            state = WhileAnalyzer::states_e::BODY_END;
            break;
        default:
            break;
    }
    return true;
}



bool IfAnalyzer::analyze_syntax() {
    init_state();
    
    if (!analyze_condition()) {
        return false;
    }

    while (!a->end_tokens()) {
        if (!a->analyze_tokens()) {
            return false;
        }

        if (section_type == -1) {
            a->pop_next();
            return true;
        }
    }

    do {
        if (!a->get_next_line()) {
            return false;
        }

        while (!a->end_tokens()) {
            if (!a->analyze_tokens()) {
                return false;
            }

            if (state == IfAnalyzer::states_e::ENDIF) {
                a->pop_next();
                return true;
            }
        }

        if (state == IfAnalyzer::states_e::ENDIF) {
            a->pop_next();
            return true;
        }
    } while (!a->eof());
    return false;
}

void IfAnalyzer::init_state() {
    state = IfAnalyzer::states_e::COND_START;
    section_type = -2;
}

bool IfAnalyzer::next_state(tokens_e token) {
    switch (state) {
        case IfAnalyzer::states_e::COND_START:
            state = IfAnalyzer::states_e::EXPR;
            break;
        case IfAnalyzer::states_e::EXPR:
            state = IfAnalyzer::states_e::THEN;
            break;
        case IfAnalyzer::states_e::THEN:
            if (token != tokens_e::THEN) {
                a->stop_interpreter("missing " + std::string(THEN_STR) + " token");
                return false;
            }
            state = IfAnalyzer::states_e::BODY;
            break;
        case IfAnalyzer::states_e::BODY:
            if (section_type == -2) {
                a->stop_interpreter("incomplete statement");
                return false;
            }
            if (section_type > 0) {
                state = IfAnalyzer::states_e::COND_START; // elif
                (*cur_index)++;
                if (!analyze_condition()) {
                    return false;
                }
                (*cur_index)--;
            } else if (section_type == 0) {
                state = IfAnalyzer::states_e::ELSE;
            } else {
                if (token != tokens_e::ENDIF) {
                    a->stop_interpreter("missing " + std::string(ENDIF_STR) + " token");
                    return false;
                }
                state = IfAnalyzer::states_e::ENDIF;
            }
            section_type = -2;
            break;
        case IfAnalyzer::states_e::ELSE:
            if (token != tokens_e::ENDIF) {
                a->stop_interpreter("missing " + std::string(ENDIF_STR) + " token");
                return false;
            }
            state = IfAnalyzer::states_e::ENDIF;
            break;
        default:
            break;
    }
    return true;
}

void IfAnalyzer::set_section(char section) {
    section_type = section;
}

bool IfAnalyzer::analyze_condition() {
    std::vector<std::string> expression;

    if (tokens->at(*cur_index)[0] == COND_START_COND) {
        next_state(tokens_e::NONE);
    }

    if (state == IfAnalyzer::states_e::COND_START) {
        if (tokens->at(*cur_index).find(COND_START_COND) == 0) {
            next_state(tokens_e::NONE);
            expression.push_back(tokens->at(*cur_index).substr(1));
        }
    }

    if (state != IfAnalyzer::states_e::EXPR) {
        a->stop_interpreter("Missing condition start ('" + std::string(1, COND_START_COND) + "')");
        return false;
    }

    if (!InstructionAnalyzer::get_condition(a, cur_index, tokens, expression)) {
        return false;
    }
    next_state(tokens_e::NONE);
    return true;
}



#define WRITE_SYNTAX_ERROR ("sytanx is '" + std::string(WRITE_STR) + " " + std::string(1, STRING_BRACKET_CHAR) + "<literals>" + std::string(1, STRING_BRACKET_CHAR) + std::string(1, WRITE_SEPARATOR) + " <variable>" + std::string(1, WRITE_SEPARATOR) + " ...'")
#define WRITE_SYNTAX_NO_ARG ("no arguments provided after '" + std::string(WRITE_STR) + "'")
#define WRITE_SYNTAX_COMMA_ERROR ("no arguments provided after '" + std::string(1, WRITE_SEPARATOR) + "'")
bool WriteAnalyzer::analyze_syntax() {
    std::string line = a->get_cur_line();
    line = line.substr(line.find_first_of(WRITE_STR) + std::string(WRITE_STR).length());

    std::size_t i = 0;
    bool is_closed = false;
    bool first_arg = true;
    bool space = false;

    i = 0;
    while (i < line.size() && std::isspace(line[i])) {
        if (!space && std::isspace(line[i])) {
            space = true;
        }
        i++;
    }

    if (space) {
        (*cur_index)++;
        space = false;
    }

    if (i >= line.size()) {
        a->stop_interpreter(WRITE_SYNTAX_NO_ARG);
        return false;
    }

    while (i < line.size()) {
        if (!first_arg) {
            if (line[i] != WRITE_SEPARATOR) {
                a->stop_interpreter(WRITE_SYNTAX_ERROR);
                return false;
            }

            for (i += 1; i < line.size() && std::isspace(line[i]); i++) {
                if (!space && std::isspace(line[i])) {
                    space = true;
                }
            }

            if (space) {
                (*cur_index)++;
                space = false;
            }

            if (i >= line.size()) {
                a->stop_interpreter(WRITE_SYNTAX_COMMA_ERROR);
                return false;
            }
        }

        first_arg = false;

        if (line[i] == STRING_BRACKET_CHAR) {
            is_closed = false;
            i++;
            std::string literal;

            while (i < line.size()) {
                if (!space && std::isspace(line[i])) {
                    space = true;
                }

                if (line[i] == STRING_ESCAPE_CHAR && i + 1 < line.size()) {
                    literal += line[i + 1];
                    i += 2;
                    if (space) {
                        (*cur_index)++;
                        space = false;
                    }
                } else if (line[i] == STRING_BRACKET_CHAR) {
                    is_closed = true;
                    i++;
                    if (space) {
                        (*cur_index)++;
                        space = false;
                    }
                    break;
                } else {
                    literal += line[i];
                    i++;
                    if (space) {
                        (*cur_index)++;
                        space = false;
                    }
                }
            }

            if (space) {
                (*cur_index)++;
                space = false;
            }

            if (!is_closed) {
                a->stop_interpreter(WRITE_SYNTAX_ERROR);
                return false;
            }

            literals.push_back(literal);
        } else {
            std::string var;

            for (; i < line.size() && line[i] != WRITE_SEPARATOR && !std::isspace(line[i]); i++) {
                if (!space && std::isspace(line[i])) {
                    space = true;
                }
                var += line[i];
            }

            if (space) {
                (*cur_index)++;
                space = false;
            }

            if (var.empty()) {
                a->stop_interpreter(WRITE_SYNTAX_COMMA_ERROR);
                return false;
            }
            
            if (var.ends_with(STRING_BRACKET_CHAR)) {
                a->stop_interpreter(WRITE_SYNTAX_ERROR);
                return false;
            }

            if (false) {
                // check var name
                a->stop_interpreter(WRITE_SYNTAX_ERROR);
                return false;
            }

            vars.push_back(var);
        }
    }

    printf("write literals\n");
    for (auto& s : literals) {
        printf("- '%s'\n", s.c_str());
    }
    printf("write vars\n");
    for (auto& s : vars) {
        printf("- '%s'\n", s.c_str());
    }

    return true;
}

void WriteAnalyzer::init_state() {}

bool WriteAnalyzer::next_state(tokens_e token) {
    return false;
}


#define READ_SYNTAX_ERROR ("sytanx is '" + std::string(READ_STR) + " <variable>" + std::string(1, READ_SEPARATOR) + " <variable>" + std::string(1, READ_SEPARATOR) + " ...'")
#define READ_SYNTAX_NO_ARG ("no arguments provided after '" + std::string(READ_STR) + "'")
#define READ_SYNTAX_COMMA_ERROR ("no arguments provided after '" + std::string(1, READ_SEPARATOR) + "'")
bool ReadAnalyzer::analyze_syntax() {
    std::string line = a->get_cur_line();
    line = line.substr(line.find_first_of(READ_STR) + std::string(READ_STR).length());

    std::size_t i = 0;
    bool first_arg = true;
    bool space = false;

    i = 0;
    while (i < line.size() && std::isspace(line[i])) {
        if (!space && std::isspace(line[i])) {
            space = true;
        }
        i++;
    }

    if (space) {
        (*cur_index)++;
        space = false;
    }

    if (i >= line.size()) {
        a->stop_interpreter(READ_SYNTAX_NO_ARG);
        return false;
    }

    while (i < line.size()) {
        if (!first_arg) {
            if (line[i] != READ_SEPARATOR) {
                a->stop_interpreter(READ_SYNTAX_ERROR);
                return false;
            }

            for (i += 1; i < line.size() && std::isspace(line[i]); i++) {
                if (!space && std::isspace(line[i])) {
                    space = true;
                }
            }

            if (space) {
                (*cur_index)++;
                space = false;
            }

            if (i >= line.size()) {
                a->stop_interpreter(READ_SYNTAX_COMMA_ERROR);
                return false;
            }
        }

        first_arg = false;

        std::string var;

        for (; i < line.size() && line[i] != READ_SEPARATOR && !std::isspace(line[i]); i++) {
            if (!space && std::isspace(line[i])) {
                space = true;
            }
            var += line[i];
        }

        if (space) {
            (*cur_index)++;
            space = false;
        }

        if (var.empty()) {
            a->stop_interpreter(READ_SYNTAX_COMMA_ERROR);
            return false;
        }
            
        if (false) {
            // check var name
            a->stop_interpreter(READ_SYNTAX_ERROR);
            return false;
        }

        vars.push_back(var);
    }
    
    printf("read vars\n");
    for (auto& s : vars) {
        printf("- '%s'\n", s.c_str());
    }

    return true;
}

void ReadAnalyzer::init_state() {}

bool ReadAnalyzer::next_state(tokens_e token) {
    return false;
}