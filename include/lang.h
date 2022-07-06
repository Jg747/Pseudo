#ifndef lang
#define lang

#include <lang_p.h>

// Random
#define TITLE_NCUR "PSEUDO v1.0-release"
#define TITLE "\n\n\n\n----------------------------- " TITLE_NCUR " ------------------------------\n\n"
#define FOOTER "--------------------------------------------------------------------------------\n\n\n\n\n\n"

// Keywords
#define BEGIN "begin"
#define END "end"               // end if | end function | end procedure
#define IF "if"
#define THEN "then"
#define ELSE "else"
#define DO "do"
#define WHILE "while"           // do .. while () | while () do ..
#define VARIABLE "variable"
#define FUNCTION "function"
#define PROCEDURE "procedure"

// Expressions
#define ASSIGN "<-"
#define RETURN_DEF_TYPE "->"
#define COMMA ","
#define TYPE ":"
#define START_INDEX "["
#define END_INDEX "]"
#define START_EXP "("
#define END_EXP ")"

// Logic operators
#define EQ "="
#define NEQ "<>"
#define MAG ">"
#define MAGEQ ">="
#define MIN "<"
#define MINEQ "<="
#define AND "and"
#define OR "or"
#define NOT "not"
#define XOR "xor"

// Math operators
#define PLUS "+"
#define MINUS "-"
#define MULTIPL "*"
#define DIVISION "/"
#define MOD "MOD"

// System calls
#define READ "input"
#define WRITE "output"
#define RETURN "return"

// Extra
#define TRIGGER_INFINITE_LOOP_ERROR 5000
#define INVALID_INPUT " [Invalid input!]"
#define END_EXEC_PGM "\n\n----- END PROGRAM -----"
#define PRESS_TO_CONTINUE "\n[press a key to continue]"
#define ERRORS "ERRORS\n\n"

#define WHITE_SPACES " \t\x01\x02\x03\x04"

#endif
