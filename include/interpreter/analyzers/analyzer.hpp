#ifndef __ANALYZER_HPP__
#define __ANALYZER_HPP__

#include <filesystem>
#include <string>

class Analyzer {
protected:
    std::filesystem::path path;
    std::size_t cur_line;
    std::size_t cur_col;
    
    Analyzer(std::string path);
public:
    void stop_interpreter(std::string msg);
    virtual bool analyze() = 0;
};

#endif // __ANALYZER_HPP__