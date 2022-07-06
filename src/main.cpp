#include <iostream>
#include <cparse/shunting-yard.h>

int main(int argc, char* argv[]) {
    std::cout << cparse::calculator::calculate("'Hello ' + 'World'") << std::endl;

    return 0;
}