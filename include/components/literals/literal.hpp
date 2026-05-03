#ifndef __LITERAL_HPP__
#define __LITERAL_HPP__

#include "value.hpp"

#include <string>
#include <memory>

class Literal {
protected:
    std::string name;
    static long id;
    std::unique_ptr<Value> val;

public:
    Literal();
    virtual ~Literal();

    void set_name(std::string& name);
    
    int get_id() const;
    std::string get_name() const;

    virtual void abstract() = 0;
};

#endif // __LITERAL_HPP__