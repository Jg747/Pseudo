#include <iostream>
#include <vector>
#include <string>

#include "program.hpp"

//#include "cparse/shunting-yard.h"
//#include "cparse/shunting-yard-exceptions.h"

int main(int argc, char **argv) {
    try {
        Program p(INTERPRETER, "test.txt");
        p.read_file();
        p.run_program();
    } catch (std::runtime_error &e) {
        std::cout << e.what();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}