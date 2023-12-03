#include "program.hpp"
#include "interpreter.hpp"
#include "exceptions.hpp"
#include "argv_split.h"

#include <string>
#include <iostream>
#include <fstream>
#include <regex>

Program::Program(enum program_mode_e mode) {
    this->mode = mode;
}

Program::Program(enum program_mode_e mode, std::string in_file) {
    this->mode = mode;
    this->in_file = in_file;
}

Program::~Program() {
    if (run != NULL) {
        delete run;
    }
}

void Program::set_input_file(std::string input) {
    in_file = input;
}

std::string Program::get_input_file() {
    return in_file;
}

void Program::read_file() {
    std::ifstream in(in_file);
    if (!in.is_open()) {
        throw ProgramException((std::size_t) 0);
    }

    std::string temp;
    for (std::getline(in, temp); in; getline(in, temp)) {
        temp = trim_string(temp);
        if (!temp.empty()) {
            content.push_back(temp);
        }
    }
}

void Program::run_program() {
    switch (mode) {
        case INTERPRETER:
            this->run = new Interpreter(content);
            break;
        case CPP_TO_PSEUDO:
            break;
        case PSEUDO_TO_CPP:
            break;
    }
    run->run();
}

std::string Program::trim_string(std::string str) {
    std::size_t start = str.find_first_not_of(WHITESPACES);
    std::size_t end = str.find_last_not_of(WHITESPACES);
    std::string ret;
    if (start == std::string::npos || end == std::string::npos) {
        ret = "";
    } else {
        ret = str.substr(start, end - start + 1);
    }
    return ret;
}

std::vector<std::string> Program::tokenize_string(std::string str) {
    argv_split parser;
    parser.parse(str);
    const char** temp = parser.argv();
    std::vector<std::string> ret;
    for (std::size_t i = 0; temp[i]; i++) {
        ret.push_back(std::string(temp[i]));
    }
    return ret;
}

std::vector<std::string> Program::split_string(std::string str, std::string regex) {
    std::vector<std::string> ret;
    std::regex rgx(regex);
    std::sregex_token_iterator iter(str.begin(), str.end(), rgx, -1);
    std::sregex_token_iterator end;
    for ( ; iter != end; iter++) {
        ret.push_back(*iter);
    }
    return ret;
}



ProgramMode::ProgramMode() {}

ProgramMode::ProgramMode(std::vector<std::string>& content) {
    this->content = content;
}

void ProgramMode::run() {}