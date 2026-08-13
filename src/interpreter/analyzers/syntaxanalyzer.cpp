/*
TODO
function support
function syntax support
*/

#include "lang.hpp"
#include "interpreter/analyzers/syntaxanalyzer.hpp"
#include "components/literals/variable.hpp"
#include "expression/expression.hpp"

#include "components/instructions/scopedinstruction.hpp"

#include "components/instructions/assignation.hpp"
#include "components/instructions/read.hpp"
#include "components/instructions/write.hpp"
#include "components/instructions/while.hpp"
#include "components/instructions/dowhile.hpp"
#include "components/instructions/if.hpp"
#include "components/instructions/return.hpp"
#include "components/instructions/call.hpp"

#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <memory>
#include <utility>
#include <cstring>
#include <expected>
#include <list>

std::list<std::unique_ptr<Instruction>>& SyntaxAnalyzer::get_instructions() {
    return instructions;
}

Instruction* SyntaxAnalyzer::get_deepest_instruction(Instruction* i, size_t deep) {
    size_t size = cur_instruction.size() - 1;
    if (size == 0) {
        return nullptr;
    }
    
    if (deep == size) {
        return i;
    }

    if (dynamic_cast<If*>(i)) {
        auto ret = get_deepest_instruction(((If*) i)->get_last_instruction(), deep + 1);
        if (!ret) {
            return i;
        }
        return ret;
    } else if (dynamic_cast<ScopedInstruction*>(i)) {
        auto ret = get_deepest_instruction(((ScopedInstruction*) i)->get_instructions().back().get(), deep + 1);
        if (!ret) {
            return i;
        }
        return ret;
    }
    return nullptr;
}

Instruction* SyntaxAnalyzer::add_instruction(std::unique_ptr<Instruction>& i) {
    Instruction* ret = i.get();
    auto last = get_deepest_instruction(instructions.back().get(), 1);
    if (dynamic_cast<ScopedInstruction*>(last)) {
        if (dynamic_cast<If*>(last)) {
            ((If*) last)->add_instruction(i);
        } else {
            ((ScopedInstruction*) last)->add_instruction(i);
        }
    } else {
        instructions.push_back(std::move(i));
    }
    return ret;
}

Instruction* SyntaxAnalyzer::add_instruction(std::unique_ptr<Instruction>&& i) {
    return this->add_instruction((std::unique_ptr<Instruction>&) i);
}

std::vector<std::string>& SyntaxAnalyzer::get_var_list() {
    return var_list;
}

void SyntaxAnalyzer::add_variable(std::string& var_name) {
    var_list.push_back(var_name);
}

bool SyntaxAnalyzer::analyze() {
    if (!get_next_line()) {
        return false;
    }

    if (parenthesis.size() > 0) {
        stop_interpreter("missing " + std::string(END_STR) + " to end block(s)");
        return false;
    }

    return true;
}

bool SyntaxAnalyzer::get_next_line() {
    std::string str;
    while (std::getline(in, str)) {
        cur_line++;
        int ret = analyze_line(str);
        if (parenthesis.size() == 0 && start) {
            break;
        }
        if (ret == 0) {
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

int SyntaxAnalyzer::next_action() {
    AssignationAnalyzer* assign = get_cur_instruction_top_ptr<AssignationAnalyzer>();
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
    return 1;
}

int SyntaxAnalyzer::analyze_tokens() {
    while (cur_index < cur_tokens.size()) {
        auto token = cur_tokens[cur_index];
        
        std::size_t comment;
        if ((comment = token.first.find_first_not_of(whitespaces)) != std::string::npos) {
            if (token.first[comment] == COMMENT_CHAR) {
                cur_index = cur_tokens.size();
                break;
            }
        }

        auto cur_tk = analyze_token(token.first);

        if (!cur_tk.has_value()) {
            stop_interpreter("Unknown token (" + token.first + ")");
            return 0;
        }

        SyntaxAnalyzer::add_instruction_ret ret = add_cur_instruction(cur_tk);

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
                if (!next_action()) {
                    return 0;
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

    if (token.value() != tokens_e::BEGIN && !start) {
        stop_interpreter("missing " + std::string(BEGIN_STR) + " to begin block");
        return SyntaxAnalyzer::add_instruction_ret::ERROR;
    }

    InstructionAnalyzer* a;

    switch (token.value()) {
        case tokens_e::BEGIN:
            if (!start) {
                start = true;
            }

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
        case tokens_e::VAR:
            var_flag = true;
            cur_instruction.push(std::make_unique<AssignationAnalyzer>());
            break;
        case tokens_e::ASSIGN:
            stop_interpreter("Error on assign expression");
            return SyntaxAnalyzer::add_instruction_ret::ERROR;
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
        case tokens_e::FUNCTION:
            stop_interpreter("'" + std::string(FUNCTION_STR) + "' not allowed in body of another function");
            return SyntaxAnalyzer::add_instruction_ret::ERROR;
        case tokens_e::RETURN:
            cur_instruction.push(std::make_unique<ReturnAnalyzer>());
            break;
        default: return SyntaxAnalyzer::add_instruction_ret::NO_TOKEN;
    }
    return SyntaxAnalyzer::add_instruction_ret::NEXT;
}

bool SyntaxAnalyzer::eof() {
    return in.eof();
}


void InstructionAnalyzer::set_params(SyntaxAnalyzer* a, std::vector<std::pair<std::string, std::size_t>>* tokens, std::size_t* index) {
    this->tokens = tokens;
    this->cur_index = index;
    this->a = a;
}

bool InstructionAnalyzer::next_state(tokens_e token) { 
    return true;
}

void InstructionAnalyzer::init_state() {}

void InstructionAnalyzer::set_begin() {
    begin = true;
}

std::expected<Expression, bool> InstructionAnalyzer::get_condition() {
    (*cur_index)--;
    
    size_t pos;
    while (*cur_index < tokens->size() && (pos = tokens->at(*cur_index).first.find(COND_START_COND)) == std::string::npos) {
        (*cur_index)++;
    }
    if (*cur_index >= tokens->size()) {
        return std::unexpected(true);
    }

    std::string cond = a->get_cur_line().substr(tokens->at(*cur_index).second + pos + 1);
    
    std::stack<bool> par;
    par.push(true);
    size_t i, spaces = 0;
    bool space = false;
    for (i = 0; i < cond.length(); i++) {
        if (std::string(1, cond[i]).find_first_of(SyntaxAnalyzer::whitespaces) != std::string::npos) {
            if (!space) {
                spaces++;
            }
            space = true;
        } else {
            space = false;
        }
        if (cond[i] == COND_START_COND) {
            par.push(true);
        }
        if (cond[i] == COND_END_COND) {
            par.pop();
        }
        if (par.empty()) {
            break;
        }
    }
    cond = cond.substr(0, i);
    (*cur_index) += spaces + 1;

    try {
        Expression e = Expression::parse_expression(cond);
        return e;
    } catch (const std::runtime_error& e) {
        a->stop_interpreter(e.what());
        return std::unexpected(false);
    }
}


bool AssignationAnalyzer::analyze_syntax() {
    if (!a->get_var_flag()) {
        a->stop_interpreter("Assignation error (need a variable to assign to)");
        return false;
    }

    auto token = tokens->at(*cur_index - 1);
    while (!a->end_tokens() && (tokens->at(*cur_index).first == ASSIGN_STR || !SyntaxAnalyzer::is_keyword(tokens->at(*cur_index).first))) {
        (*cur_index)++;
    }
    auto end = tokens->at(*cur_index - 1);
    size_t count = end.second + end.first.length() - token.second;
    std::string line = a->get_cur_line().substr(token.second, count);

    if (line.find(ASSIGN_STR) != std::string::npos) {
        if (!assignment(line)) {
            return false;
        }
    } else if (line.find(FUNC_START_ARGS) != std::string::npos && line.find(FUNC_END_ARGS) != std::string::npos) {
        if (!create_call(line)) {
            return false;
        }
    } else {
        a->stop_interpreter("Assignation error: invalid expression provided");
        return false;
    }

    a->pop_next();
    return true;
}

bool AssignationAnalyzer::create_instruction() {
    try {
        auto l_vars = Expression::parse_expression(l).req_vars();
        if (l_vars.size() == 0) {
            a->stop_interpreter("Error on left side of expression (var to assign)");
            return false;
        }
        // la prima variabile e' sempre quella a cui viene assegnato il valore
        // le altre sono operatori usati in caso sia una cella di un array
        a->add_variable(l_vars[0]);

        Expression expr = Expression::parse_expression(l + " = " + r);
        a->add_instruction(std::make_unique<Assignation>(expr));
    } catch (const std::runtime_error& e) {
        a->stop_interpreter("Assignation error: " + std::string(e.what()));
        return false;
    }
    return true;
}

bool AssignationAnalyzer::assignment(std::string& line) {
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

    if (r.find(COMMENT_CHAR) != std::string::npos) {
        r = r.substr(0, r.find(COMMENT_CHAR) - 1);
    }

    if (!create_instruction()) {
        return false;
    }
    
    return true;
}

bool AssignationAnalyzer::create_call(std::string& line) {
    if (line.find(COMMENT_CHAR) != std::string::npos) {
        line = line.substr(0, line.find(COMMENT_CHAR) - 1);
    }

    try {
        Expression expr = Expression::parse_expression(line);
        a->add_instruction(std::make_unique<Call>(expr));
    } catch (const std::runtime_error& e) {
        a->stop_interpreter("Assignation error: " + std::string(e.what()));
        return false;
    }
    return true;
}


bool UntilAnalyzer::analyze_syntax() {
    init_state();
    pointer = (DoWhile*) a->add_instruction(std::make_unique<DoWhile>());

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

    create_instruction();

    a->pop_next();
    return true;
}

void UntilAnalyzer::init_state() {
    state = UntilAnalyzer::states_e::BODY;
}

bool UntilAnalyzer::next_state(tokens_e token) {
    switch (state) {
        case UntilAnalyzer::states_e::BODY:
            state = UntilAnalyzer::states_e::UNTIL;
            break;
        default:
            break;
    }
    return true;
}

bool UntilAnalyzer::create_instruction() {
    try {
        auto expr = get_condition();
        if (expr.has_value()) {
            pointer->set_condition(expr.value());
        } else {
            throw std::runtime_error("get_condition() returned no value");
        }
    } catch (const std::runtime_error& e) {
        a->stop_interpreter("Error in condition");
        return false;
    }

    return true;
}


bool WhileAnalyzer::analyze_syntax() {
    init_state();

    create_instruction();

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
        a->pop_next();
        return true;
    }

    a->stop_interpreter("need " + std::string(BEGIN_STR) + " after " + std::string(WHILE_STR) + " statement");
    return false;
}

void WhileAnalyzer::init_state() {
    state = WhileAnalyzer::states_e::BODY_BEGIN;
}

bool WhileAnalyzer::next_state(tokens_e token) {
    switch (state) {
        case WhileAnalyzer::states_e::BODY_BEGIN:
            state = WhileAnalyzer::states_e::WAIT_END;
            break;
        case WhileAnalyzer::states_e::WAIT_END:
            state = WhileAnalyzer::states_e::BODY_END;
            break;
        default:
            break;
    }
    return true;
}

bool WhileAnalyzer::create_instruction() {
    try {
        auto expr = get_condition();
        if (expr.has_value()) {
            pointer = (While*) a->add_instruction(std::make_unique<While>(expr.value()));
        } else {
            throw std::runtime_error("get_condition() returned no value");
        }
    } catch (const std::runtime_error& e) {
        a->stop_interpreter("Error in condition");
        return false;
    }

    return true;
}



bool IfAnalyzer::analyze_syntax() {
    init_state();

    create_instruction();
    
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
                Expression e = Expression::parse_expression("1");
                pointer->add_sequence(e);
            } else {
                if (token != tokens_e::ENDIF) {
                    a->stop_interpreter("missing " + std::string(ENDIF_STR) + " token");
                    return false;
                }
                state = IfAnalyzer::states_e::ENDIF;
            }
            section_type = -2;
            break;
        case IfAnalyzer::states_e::ELSE: {
            if (token != tokens_e::ENDIF) {
                a->stop_interpreter("missing " + std::string(ENDIF_STR) + " token");
                return false;
            }
            state = IfAnalyzer::states_e::ENDIF;
            break;
        }
        default:
            break;
    }
    return true;
}

void IfAnalyzer::set_section(char section) {
    section_type = section;
}

bool IfAnalyzer::analyze_condition() {
    try {
        auto expr = get_condition();
        if (expr.has_value()) {
            pointer->add_sequence(expr.value());
        } else {
            throw std::runtime_error("get_condition() returned no value");
        }
    } catch (const std::runtime_error& e) {
        a->stop_interpreter("Error in condition");
        return false;
    }
    next_state(tokens_e::NONE);
    return true;
}

bool IfAnalyzer::create_instruction() {
    pointer = (If*) a->add_instruction(std::make_unique<If>());
    return true;
}



static char get_escaped(char c) {
    switch (c) {
        case 'n':
            return '\n';
        case 'r':
            return '\r';
        case 't':
            return '\t';
        case 'b':
            return '\b';
        case 'f':
            return '\f';
        case 'v':
            return '\v';
        default:
            return c;
    }
}

void WriteAnalyzer::init() {
    line = a->get_cur_line();
    line = line.substr(line.find_first_of(WRITE_STR) + std::string(WRITE_STR).length());

    i = 0;
    is_closed = false;
    first_arg = true;
    space = false;

    (*cur_index)--;
}

void WriteAnalyzer::skip_first_spaces() {
    while (i < line.size() && std::isspace(line[i])) {
        if (!space && std::isspace(line[i])) {
            space = true;
            (*cur_index)++;
        }
        i++;
    }

    space = false;
}

void WriteAnalyzer::skip_spaces() {
    for (i += 1; i < line.size() && std::isspace(line[i]); i++) {
        if (!space && std::isspace(line[i])) {
            space = true;
            (*cur_index)++;
        }
    }

    space = false;
}

bool WriteAnalyzer::comma() {
    if (line[i] != WRITE_SEPARATOR) {
        a->stop_interpreter(WRITE_SYNTAX_ERROR);
        return false;
    }

    skip_spaces();

    if (i >= line.size() || line[i] == COMMENT_CHAR) {
        a->stop_interpreter(WRITE_SYNTAX_COMMA_ERROR);
        return false;
    }

    return true;
}

bool WriteAnalyzer::parse_literal() {
    is_closed = false;
    i++;
    std::string literal;

    while (i < line.size()) {
        if (!space && std::isspace(line[i])) {
            (*cur_index)++;
            space = true;
        } else if (space && !std::isspace(line[i])) {
            space = false;
        }

        if (line[i] == STRING_ESCAPE_CHAR && i + 1 < line.size()) {
            literal += get_escaped(line[i + 1]);
            i += 2;
        } else if (line[i] == STRING_BRACKET_CHAR) {
            is_closed = true;
            i++;
            break;
        } else {
            literal += line[i];
            i++;
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

    literals.push_back({ .lit = literal, .is_variable = false });

    skip_spaces();
    
    return true;
}

std::string WriteAnalyzer::get_expression() {
    std::string var = "";
    bool func = false;
    for (; i < line.size(); i++) {
        if (!space && std::isspace(line[i])) {
            (*cur_index)++;
            space = true;
        } else if (space && !std::isspace(line[i])) {
            space = false;
        }
        
        if (line[i] == FUNC_START_ARGS) {
            func = true;
        }

        if (line[i] == FUNC_END_ARGS) {
            func = false;
        }

        if (!func && line[i] == WRITE_SEPARATOR) {
            break;
        }

        var += line[i];
    }
    SyntaxAnalyzer::trim_string(var);
    return var;
}

bool WriteAnalyzer::check_var_name(std::string& var) {
    if (var.contains("?") || var.contains("[") || var.contains(FUNC_START_ARGS)) {
        std::string temp;
        if (var.contains("[")) {
            temp = var.substr(0, var.find("["));
        } else if (var.contains("?")) {
            temp = var.substr(0, var.find("?"));
        } else {
            temp = var.substr(0, var.find(FUNC_START_ARGS));
        }

        SyntaxAnalyzer::trim_string(temp);
                
        if (!Variable::is_name_correct(temp)) {
            a->stop_interpreter("'" + var + "' is not a valid variable name");
            return false;
        }
    } else {
        if (!Variable::is_name_correct(var)) {
            a->stop_interpreter("'" + var + "' is not a valid variable name");
            return false;
        }
    }
    return true;
}

bool WriteAnalyzer::parse_expression() {
    std::string var = get_expression();
    if (var.contains(COMMENT_CHAR)) {
        var = var.substr(0, var.find(COMMENT_CHAR) - 1);
    }

    if (var.empty()) {
        a->stop_interpreter(WRITE_SYNTAX_COMMA_ERROR);
        return false;
    }
            
    if (var.ends_with(STRING_BRACKET_CHAR)) {
        a->stop_interpreter(WRITE_SYNTAX_ERROR);
        return false;
    }

    if (!check_var_name(var)) {
        return false;
    }

    literals.push_back({ .lit = var, .is_variable = true });

    return true;
}

bool WriteAnalyzer::analyze_syntax() {
    init();

    skip_first_spaces();

    if (i >= line.size()) {
        a->stop_interpreter(WRITE_SYNTAX_NO_ARG);
        return false;
    }

    while (i < line.size()) {
        if (!first_arg) {
            if (!comma()) {
                return false;
            }
        }

        first_arg = false;

        if (line[i] == STRING_BRACKET_CHAR) {
            if (!parse_literal()) {
                return false;
            }
        } else {
            if (!parse_expression()) {
                return false;
            }
        }

        if (line[i] == COMMENT_CHAR) {
            break;
        }
    }

    create_instruction();
    a->pop_next();

    (*cur_index)++;

    return true;
}

bool WriteAnalyzer::create_instruction() {
    std::list<std::shared_ptr<WriteLiteral>> print;
    for (auto& s : literals) {
        if (s.is_variable) {
            try {
                print.push_back(std::make_shared<WriteExpr>(Expression::parse_expression(s.lit)));
            } catch (const std::runtime_error& e) {
                a->stop_interpreter(e.what());
                return false;
            }
        } else {
            print.push_back(std::make_shared<WriteString>(s.lit));
        }
    }
    a->add_instruction(std::make_unique<Write>(print));
    return true;
}

void WriteAnalyzer::init_state() {}

bool WriteAnalyzer::next_state(tokens_e token) {
    return false;
}



void ReadAnalyzer::init() {
    line = a->get_cur_line();
    line = line.substr(line.find_first_of(READ_STR) + std::string(READ_STR).length());

    i = 0;
    first_arg = true;
    space = false;
}

void ReadAnalyzer::skip_first_spaces() {
    while (i < line.size() && std::isspace(line[i])) {
        if (!space && std::isspace(line[i])) {
            space = true;
            (*cur_index)++;
        }
        i++;
    }

    space = false;
}

void ReadAnalyzer::skip_spaces() {
    for (i += 1; i < line.size() && std::isspace(line[i]); i++) {
        if (!space && std::isspace(line[i])) {
            space = true;
            (*cur_index)++;
        }
    }

    space = false;
}

bool ReadAnalyzer::comma() {
    if (line[i] != READ_SEPARATOR) {
        a->stop_interpreter(READ_SYNTAX_ERROR);
        return false;
    }

    skip_spaces();

    if (i >= line.size()) {
        a->stop_interpreter(READ_SYNTAX_COMMA_ERROR);
        return false;
    }

    return true;
}

std::string ReadAnalyzer::get_expression() {
    std::string var = "";
    for (; i < line.size() && line[i] != READ_SEPARATOR; i++) {
        if (!space && std::isspace(line[i])) {
            (*cur_index)++;
            space = true;
        } else if (space && !std::isspace(line[i])) {
            space = false;
        }
        var += line[i];
    }
    SyntaxAnalyzer::trim_string(var);
    return var;
}

bool ReadAnalyzer::check_var_name(std::string& var) {
    if (var.contains("?") || var.contains("[")) {
        std::string temp;
        if (var.contains("[")) {
            temp = var.substr(0, var.find("["));
        } else {
            temp = var.substr(0, var.find("?"));
        }

        SyntaxAnalyzer::trim_string(temp);
                
        if (!Variable::is_name_correct(temp)) {
            a->stop_interpreter("'" + var + "' is not a valid variable name");
            return false;
        }
    } else {
        if (!Variable::is_name_correct(var)) {
            a->stop_interpreter("'" + var + "' is not a valid variable name");
            return false;
        }
    }
    return true;
}

bool ReadAnalyzer::parse_expression() {
    std::string var = get_expression();
    if (var.contains(COMMENT_CHAR)) {
        var = var.substr(0, var.find(COMMENT_CHAR) - 1);
    }

    if (var.empty()) {
        a->stop_interpreter(READ_SYNTAX_COMMA_ERROR);
        return false;
    }

    if (!check_var_name(var)) {
        return false;
    }

    std::string expr = var + " = " + std::string(READ_VAR);
    try {
        vars.push_back({ var, Expression::parse_expression(expr) });
    } catch (const std::runtime_error& e) {
        a->stop_interpreter(e.what());
        return false;
    }

    return true;
}

bool ReadAnalyzer::analyze_syntax() {
    init();

    skip_first_spaces();

    if (i >= line.size()) {
        a->stop_interpreter(READ_SYNTAX_NO_ARG);
        return false;
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
    }

    create_instruction();
    a->pop_next();
    return true;
}

void ReadAnalyzer::init_state() {}

bool ReadAnalyzer::next_state(tokens_e token) {
    return false;
}

bool ReadAnalyzer::create_instruction() {
    // Read can create variables like an assignation
    for (auto& v : vars) {
        if (v.first != READ_VAR) {
            a->add_variable(v.first);
        }
    }

    a->add_instruction(std::make_unique<Read>(vars));
    return true;
}



bool ReturnAnalyzer::analyze_syntax() {
    if (a->end_tokens() || SyntaxAnalyzer::is_keyword(tokens->at(*cur_index).first) || tokens->at(*cur_index).first.find(COMMENT_CHAR) == 0) {
        this->expr = "";
        return create_instruction();
    }

    auto token = tokens->at(*cur_index);
    while (!a->end_tokens() && !SyntaxAnalyzer::is_keyword(tokens->at(*cur_index).first)) {
        (*cur_index)++;
    }
    auto end = tokens->at(*cur_index - 1);
    size_t count = end.second + end.first.length() - token.second;
    this->expr = a->get_cur_line().substr(token.second, count);

    if (this->expr.find(COMMENT_CHAR) != std::string::npos) {
        this->expr = this->expr.substr(0, this->expr.find(COMMENT_CHAR) - 1);
    }

    return create_instruction();
}

void ReturnAnalyzer::init_state() {}

bool ReturnAnalyzer::next_state(tokens_e token) { return false; }

bool ReturnAnalyzer::create_instruction() {
    Expression e;
    if (expr.empty()) {
        a->add_instruction(std::make_unique<Return>(e));
        a->pop_next();
        return true;
    }
    
    try {
        e = Expression::parse_expression(expr);
        a->add_instruction(std::make_unique<Return>(e));
        a->pop_next();
    } catch (const std::runtime_error& e) {
        a->stop_interpreter(e.what());
        return false;
    }

    return true;
}