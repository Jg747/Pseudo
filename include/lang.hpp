/* -------- LANG -------- */
#ifndef __LANG_HPP__
#define __LANG_HPP__

enum class tokens_e {
    BEGIN,
    END,
    IF,
    THEN,
    ELIF,
    ELSE,
    ENDIF,
    WHILE,
    ENDWHILE,
    REPEAT,
    UNTIL,
    READ,
    WRITE,
    ASSIGN,
    FUNCTION,
    RETURN,
    OPERATOR,
    EXPR_TOK,
    VAR,
    NONE
};

#define BEGIN_STR                       "BEGIN"
#define END_STR                         "END"
#define IF_STR                          "IF"
#define THEN_STR                        "THEN"
#define ELIF_STR                        "ELIF"
#define ELSE_STR                        "ELSE"
#define ENDIF_STR                       "ENDIF"
#define WHILE_STR                       "WHILE"
#define ENDWHILE_STR                    "ENDWHILE"
#define REPEAT_STR                      "REPEAT"
#define UNTIL_STR                       "UNTIL"
#define READ_STR                        "read"
#define WRITE_STR                       "write"
#define ASSIGN_STR                      "<-"
#define FUNCTION_STR                    "FUNCTION"
#define RETURN_STR                      "RETURN"

#define ALLOWED_VARSFUNCS_CHARS         "^[A-Za-z_][A-Za-z_0-9]*$"
#define ALLOWED_OPERATORS               "^(<=)$|^(>=)$|^(==)$|^(!=)$|^([\\+\\-\\*\\/\\%<>])$|^(\\[[0-9]+\\])$|^(\\^)$|^(\\$)$|^(\\?)$" // arr^x add x, arr$index remove, arr? arr_size
#define EXPR_CHARS                      "^(([0-9]+)|([0-9]+\\.[0-9]+))$"
#define STRING_BRACKET_CHAR             '\"'
#define STRING_ESCAPE_CHAR              '\\'
#define WRITE_SEPARATOR                 ','
#define READ_SEPARATOR                  ','
#define FUNC_START_ARGS                 '('
#define FUNC_END_ARGS                   ')'
#define COND_START_COND                 '('
#define COND_END_COND                   ')'

#endif // __LANG_HPP__
//^(<=)|(>=)|(==)|(!=)|([\+\-\*\/\%<>])|(\[[0-9]+\])|(\^)|(\$)|(\?)$