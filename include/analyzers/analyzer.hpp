#ifndef __ANALYZER_HPP__
#define __ANALYZER_HPP__

#include <filesystem>
#include <string>

class Analyzer {
protected:
    std::filesystem::path path;
    
    Analyzer(std::string path);
public:
    virtual bool analyze() = 0;
};

#endif // __ANALYZER_HPP__