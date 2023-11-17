#include <iostream>

#include "cparse/shunting-yard.h"
#include "cparse/shunting-yard-exceptions.h"

using namespace cparse;

int main(int argc, char **argv) {
    std::string test = std::string(argv[1]);
    std::cout << calculator::calculate(test.c_str()) << std::endl;
}