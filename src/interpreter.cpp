#include "interpreter.hpp"
#include "program.hpp"
#include "symbols.hpp"
#include "exceptions.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <stack>

using std::cout;
using std::string;
using std::vector;
using std::stack;
using std::size_t;

Interpreter::Interpreter() {}

Interpreter::Interpreter(std::vector<std::string> &content) {
    f_content = content;
}

void Interpreter::run() {
    check_syntax();
    create_codepoint();
    execute();
}

void Interpreter::check_syntax() {
    for (size_t i = 0; i < f_content.size(); i++) {
        vector<string> tokens = Program::tokenize_string(f_content[i]);
        check_row(i, tokens);
    }
    
    if (!parenthesis.empty()) {
        throw SyntaxException((std::size_t) 0, 0);
    }
}

void Interpreter::check_row(std::size_t row, vector<string> &tokens) {
    size_t tok_size = tokens.size();
    for (string token : tokens) {
        if (token == INIZIO) {
            if (tok_size > 1) {
                throw SyntaxException(row, 1);
            }
            parenthesis.push('{');
        } else if (token == FINE) {
            if (tok_size > 1) {
                throw SyntaxException(row, 1);
            }
            parenthesis.pop();
        } else if (token == SE) {
            
        } else if (token == ALLORA) {
            if (tok_size > 1) {
                throw SyntaxException(row, 1);
            }
        } else if (token == ALTRIMENTI) {
            if (tok_size > 1) {
                throw SyntaxException(row, 1);
            }
        } else if (token == FINE_SE) {
            if (tok_size > 1) {
                throw SyntaxException(row, 1);
            }
        } else if (token == RIPETI) {
            if (tok_size > 1) {
                throw SyntaxException(row, 1);
            }
        } else if (token == FINO_A_CHE) {
            
        } else if (token == FINTANTOCHE) {
            
        } else if (token == ESEGUI) {

        } else if (token == LEGGI) {
            if (tok_size == 1) {
                throw SyntaxException(row, 2);
            }
        } else if (token == SCRIVI) {
            if (tok_size == 1) {
                throw SyntaxException(row, 2);
            }
        } else {

        }
    }
}

void Interpreter::create_codepoint() {

}

void Interpreter::execute() {

}