#include "exceptions.hpp"

#include <exception>
#include <string>

CustomException::CustomException(int code, std::string msg) : runtime_error(msg) {
    this->code = code;
}

const char* CustomException::what() const noexcept {
    std::string temp = std::string("[Code: " + std::to_string(code + 1) + "] " + runtime_error::what());
    char *ret = new char[temp.length() + 1];
    strcpy(ret, temp.c_str());
    return ret;
}


VariableException::VariableException(const char* message) : CustomException(-1, std::string(message)) {
    line = -1;
}

VariableException::VariableException(const std::string message) : CustomException(-1, message) {
    line = -1;
}

VariableException::VariableException(std::size_t index) : CustomException(index, std::string(errors[index])) {
    line = -1;
}

VariableException::VariableException(const char* message, std::size_t line) : CustomException(-1, std::string(message)) {
    this->line = line;
}

VariableException::VariableException(const std::string message, std::size_t line) : CustomException(-1, message) {
    this->line = line;
}

VariableException::VariableException(std::size_t index, std::size_t line) : CustomException(index, errors[index]) {
    this->line = line;
}

const char* VariableException::what() const noexcept {
    std::string temp = std::string("[Line: " + std::to_string(line) + "] " + CustomException::what());
    char *ret = new char[temp.length() + 1];
    strcpy(ret, temp.c_str());
    return ret;
}



ProgramException::ProgramException(const char* message) : CustomException(-1, std::string(message)) {}

ProgramException::ProgramException(const std::string message) : CustomException(-1, message) {}

ProgramException::ProgramException(std::size_t index) : CustomException(index, errors[index]) {}



SyntaxException::SyntaxException(const char* message) : CustomException(-1, std::string(message)) {
    line = -1;
}

SyntaxException::SyntaxException(const std::string message) : CustomException(-1, message) {
    line = -1;
}

SyntaxException::SyntaxException(std::size_t index) : CustomException(index, std::string(errors[index])) {
    line = -1;
}

SyntaxException::SyntaxException(const char* message, std::size_t line) : CustomException(-1, std::string(message)) {
    this->line = line;
}

SyntaxException::SyntaxException(const std::string message, std::size_t line) : CustomException(-1, message) {
    this->line = line;
}

SyntaxException::SyntaxException(std::size_t index, std::size_t line) : CustomException(index, errors[index]) {
    this->line = line;
}

const char* SyntaxException::what() const noexcept {
    std::string temp = std::string("[Line: " + std::to_string(line) + "] " + CustomException::what());
    char *ret = new char[temp.length() + 1];
    strcpy(ret, temp.c_str());
    return ret;
}