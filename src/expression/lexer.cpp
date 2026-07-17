#include "expression/lexer.hpp"

#include <cctype>
#include <stdexcept>

bool Lexer::isIdentifierStart(char c) const {
    return std::isalpha(static_cast<unsigned char>(c)) || c == '_';
}

bool Lexer::isIdentifier(char c) const {
    return std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '.';
}

void Lexer::replace_all(std::string& str, std::string sub, std::string with) const {
    if (str.empty()) {
        return;
    }

    size_t pos = 0;
    while ((pos = str.find(sub)) != std::string::npos) {
        str.replace(pos, sub.length(), with);
    }
}

void Lexer::process_number() {
    std::size_t start = i;
    bool dot = false;

    if (c == '.') {
        dot = true;
    }

    i++;
    while (i < expression.size()) {
        char current = expression[i];

        if (std::isdigit(current)) {
            i++;
        } else if (current == '.') {
            if (dot) {
                throw std::runtime_error("Invalid number.");
            }

            dot = true;
            i++;
        } else {
            break;
        }
    }

    auto text = expression.substr(start, i - start);

    if (text == ".") {
        throw std::runtime_error("Invalid number.");
    }

    tokens.emplace_back(std::stod(text));
}

void Lexer::process_identifier() {
    std::size_t start = i;

    i++;
    while (i < expression.size() && isIdentifier(expression[i])) {
        i++;
    }
            
    std::string s = expression.substr(start, i - start);
            
    if (s == "AND") {
        tokens.emplace_back(token_t::LogicalAnd);
    } else if (s == "OR") {
        tokens.emplace_back(token_t::LogicalOr);
    } else {
        tokens.emplace_back(s);
    }
}

void Lexer::process_special() {
    switch (c) {
        case '+':
            tokens.emplace_back(token_t::Plus);
            break;
        case '*':
            tokens.emplace_back(token_t::Multiply);
            break;
        case '/':
            tokens.emplace_back(token_t::Divide);
            break;
        case '(':
            tokens.emplace_back(token_t::LeftParen);
            break;
        case ')':
            tokens.emplace_back(token_t::RightParen);
            break;
        case '%':
            tokens.emplace_back(token_t::Modulo);
            break;
        case '<': {
            i++;
            if (expression[i] == '=') {
                tokens.emplace_back(token_t::LessEqual);
            } else {
                tokens.emplace_back(token_t::Less);
                i--;
            }
            break;
        }
        case '>': {
            i++;
            if (expression[i] == '=') {
                tokens.emplace_back(token_t::GreaterEqual);
            } else {
                tokens.emplace_back(token_t::Greater);
                i--;
            }
            break;
        }
        case '=': {
            i++;
            if (expression[i] == '=') {
                tokens.emplace_back(token_t::Equal);
            } else {
                throw std::runtime_error("Expected '==', provided '='");
            }
            break;
        }
        case '!': {
            i++;
            if (expression[i] == '=') {
                tokens.emplace_back(token_t::NotEqual);
            } else {
                throw std::runtime_error("Expected '!=', provided '!'");
            }
            break;
        }
        case '-': {
            const op_info* prec = Token::get_op_info(tokens.back().type);
            bool unary =    tokens.empty() || 
                            tokens.back().type == token_t::LeftParen || 
                            (prec != nullptr && prec->position != op_pos::Postfix);
            if (unary)
                tokens.emplace_back(token_t::UnaryMinus);
            else
                tokens.emplace_back(token_t::Minus);
            break;
        }
        case '?':
            tokens.emplace_back(token_t::ArraySize);
            break;
        case '[':
            tokens.emplace_back(token_t::LeftBracket);
            break;
        case ']':
            tokens.emplace_back(token_t::RightBracket);
            break;
        case '"':
            process_string();
            break;
        default:
            throw std::runtime_error(std::string("Unexpected character: ") + c);
    }
}

void Lexer::process_string() {
    i++;
    size_t start = i;
    std::string str = "";
    while (i < expression.size()) {
        if (expression[i] == '"' && expression[i - 1] != '\\') {
            break;
        }
        i++;
    }
    
    auto text = expression.substr(start, i - start);
    replace_all(text, "\\\"", "\"");
    tokens.emplace_back(Token(text, true));
}

std::vector<Token> Lexer::tokenize(const std::string& expression) {
    this->expression = expression;
    tokens.clear();
    i = 0;

    while (i < expression.size()) {
        c = expression[i];

        if (std::isspace(static_cast<unsigned char>(c))) {
            i++;
            continue;
        }

        if (std::isdigit(c) || c == '.') {
            process_number();
            continue;
        }

        if (isIdentifierStart(c)) {
            process_identifier();
            continue;
        }

        process_special();

        i++;
    }

    return tokens;
}