#include <iostream>

#include "interpreter.hpp"
#include "analyzers/syntaxanalyzer.hpp"
#include "components/literals/value.hpp"
#include "components/literals/numbervalue.hpp"
#include "components/literals/stringvalue.hpp"
#include "components/literals/arrayvalue.hpp"
#include "components/literals/variable.hpp"
#include "expression/lexer.hpp"
#include "expression/expression.hpp"
#include "expression/parser.hpp"
#include "expression/token.hpp"
#include "expression/variablecontext.hpp"

int main(int argc, char** argv) {
    using namespace std;

    SyntaxAnalyzer an("file.txt");
    if (an.analyze()) {
        cout << "\nOK!";
    } else {
        cout << "\nNON OK!";
    }

    /*if (argc != 2) {
        return 1;
    }*/
    /*Lexer l;
    std::vector<Token> tokens = l.tokenize(std::string(argv[1]));
    Parser p;
    Expression e = p.parse(tokens);
    auto tokens2 = e.rpn();
    for (auto t : tokens2) {
        std::cout << t << std::endl;
    }*/
    /*Expression e = Expression::parse_expression(std::string(argv[1]));

    VariableContext v;
    ArrayValue arr;
    arr.add_value(NumberValue(5));
    arr.add_value(NumberValue(10));
    arr.add_value(NumberValue(15));
    Variable ciao(std::string("ciao"), arr);
    Variable thomas(std::string("thomas"), StringValue("Brotherro"));
    v.set(std::make_shared<Variable>(ciao));
    v.set(std::make_shared<Variable>(thomas));
    std::cout << e.evaluate(v)->get_value();*/

    return 0;
}