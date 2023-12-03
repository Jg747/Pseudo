/*
Execute code flow
*/

#pragma once

#include "program.hpp"

#include <vector>
#include <stack>

class Interpreter : public ProgramMode {
    private:
        std::vector<std::string> f_content;
        std::stack<char> parenthesis;

        void check_row(std::size_t row, std::vector<std::string> &tokens);

    public:
        Interpreter();
        Interpreter(std::vector<std::string> &content);
        
        void run();
        void check_syntax();
        void create_codepoint();
        void execute();
};