#include "analyzers/analyzer.hpp"

#include <iostream>
#include <filesystem>

Analyzer::Analyzer(std::string path) {
    cur_line = 0;
    cur_col = 0;

    if (std::filesystem::exists(path)) {
        this->path = std::filesystem::path(path);
    } else {
        stop_interpreter("Provided path doesn't exist");
        return;
    }
}

void Analyzer::stop_interpreter(std::string msg) {
    std::cout << "[ERROR] (line " << cur_line << ") " << msg << std::endl;
}