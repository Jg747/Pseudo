#include "interpreter/analyzers/analyzer.hpp"
#include "lang.hpp"

#include <iostream>
#include <filesystem>
#include <regex>
#include <unordered_map>
#include <optional>

const char* Analyzer::whitespaces = " \n\t";
const std::regex Analyzer::var_regex(ALLOWED_VARS_CHARS);
std::unordered_map<tokens_e, std::regex> Analyzer::keywords;

void Analyzer::load_keywords() {
    keywords.insert({ tokens_e::BEGIN, std::regex("^" + std::string(BEGIN_STR) + "$") });
    keywords.insert({ tokens_e::END, std::regex("^" + std::string(END_STR) + "$") });
    keywords.insert({ tokens_e::IF, std::regex("^" + std::string(IF_STR) + "(\\(.*)?$") });
    keywords.insert({ tokens_e::THEN, std::regex("^" + std::string(THEN_STR) + "$") });
    keywords.insert({ tokens_e::ELIF, std::regex("^" + std::string(ELIF_STR) + "(\\(.*)?$") });
    keywords.insert({ tokens_e::ELSE, std::regex("^" + std::string(ELSE_STR) + "$") });
    keywords.insert({ tokens_e::ENDIF, std::regex("^" + std::string(ENDIF_STR) + "$") });
    keywords.insert({ tokens_e::WHILE, std::regex("^" + std::string(WHILE_STR) + "(\\(.*)?$") });
    keywords.insert({ tokens_e::ENDWHILE, std::regex("^" + std::string(ENDWHILE_STR) + "$") });
    keywords.insert({ tokens_e::REPEAT, std::regex("^" + std::string(REPEAT_STR) + "$") });
    keywords.insert({ tokens_e::UNTIL, std::regex("^" + std::string(UNTIL_STR) + "(\\(.*)?$") });
    keywords.insert({ tokens_e::READ, std::regex("^" + std::string(READ_STR) + "$") });
    keywords.insert({ tokens_e::WRITE, std::regex("^" + std::string(WRITE_STR) + "$") });
    keywords.insert({ tokens_e::ASSIGN, std::regex("^" + std::string(ASSIGN_STR) + "$") });
    keywords.insert({ tokens_e::FUNCTION, std::regex("^" + std::string(FUNCTION_STR) + "$") });
    keywords.insert({ tokens_e::RETURN, std::regex("^" + std::string(RETURN_STR) + "$") });
}

Analyzer::Analyzer(std::ifstream& in, std::size_t& cur_line) : in(in), cur_line(cur_line) {
    load_keywords();
}

void Analyzer::stop_interpreter(std::string msg) {
    std::cout << "[ERROR] (line " << cur_line << ") " << msg << std::endl;
}

std::vector<std::pair<std::string, std::size_t>> Analyzer::tokenize_string(std::string string) {
    std::istringstream stream(string);
    std::vector<std::pair<std::string, std::size_t>> ret;
    std::string token;
    while (std::getline(stream, token, ' ')) {
        std::streampos pos = stream.tellg();
        if (token.size() > 0) {
            if (pos >= 0) {
                ret.push_back({ token, (static_cast<std::size_t>(static_cast<std::streamoff>(pos)) - token.length() - 1) });
            } else {
                ret.push_back({ token, (string.length() - token.length()) });
            }
        }
    }

    for (auto& s : ret) {
        trim_string(s.first);
    }

    return ret;
}

void Analyzer::trim_string(std::string& string) {
    std::size_t start = string.find_first_not_of(whitespaces);
    if (start == std::string::npos) {
        return;
    }

    std::size_t end = string.find_last_not_of(whitespaces);
    string = string.substr(start, end - start + 1);
}

bool Analyzer::is_keyword(std::string& token) {
    for (auto& [k, v] : keywords) {
        if (std::regex_match(token, v)) {
            return true;
        }
    }
    return false;
}

std::string Analyzer::get_cur_line() {
    return _cur_line;
}

std::size_t& Analyzer::get_cur_line_number() {
    return cur_line;
}

std::ifstream& Analyzer::get_file() {
    return in;
}

std::optional<tokens_e> Analyzer::analyze_token(std::string& token) {
    if (token.empty() || token.size() == 0) {
        return {};
    }

    for (auto& [t, r] : keywords) {
        if (std::regex_match(token, r)) {
            return t;
        }
    }

    if (std::regex_match(token, var_regex)) {
        return tokens_e::VAR;
    }
    return tokens_e::NONE;
}