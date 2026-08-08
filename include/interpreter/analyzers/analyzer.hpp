#ifndef __ANALYZER_HPP__
#define __ANALYZER_HPP__

#include "lang.hpp"

#include <unordered_map>
#include <fstream>
#include <string>
#include <regex>
#include <optional>

class Analyzer {
protected:
    std::ifstream& in;
    std::size_t& cur_line;
    std::size_t cur_col;
    std::string _cur_line;
    
    Analyzer(std::ifstream& in, std::size_t& cur_line);

    static const std::regex var_regex;
    static std::unordered_map<tokens_e, std::regex> keywords;
    
    static void load_keywords();

public:
    static const char* whitespaces;

    static std::optional<tokens_e> analyze_token(std::string& token);
    static std::vector<std::pair<std::string, std::size_t>> tokenize_string(std::string string);
    static void trim_string(std::string& string);
    static bool is_keyword(std::string& token);

    void stop_interpreter(std::string msg);
    std::string get_cur_line();
    std::size_t& get_cur_line_number();
    std::ifstream& get_file();
    virtual bool analyze() = 0;
};

#endif // __ANALYZER_HPP__