/*
Contains objects to represent loops
- while
- do while
- for
*/

#pragma once

#include <string>

enum loop_type_e {
    WHILE, // for
    DOWHILE
};

class Loop {
    protected:
        enum loop_type_e type;
        std::string condition;
        std::size_t start_idx;
        std::size_t end_idx;
        std::size_t jump_false;
    
    public:
        Loop();
        Loop(enum loop_type_e type);

        void set_condition(std::string condition);
        void set_start_idx(std::size_t start_idx);
        void set_end_idx(std::size_t end_idx);
        void set_jump_false(std::size_t jump_false);

        std::string get_condition();
        std::size_t get_start_idx();
        std::size_t get_end_idx();
        std::size_t get_jump_false();
};

class For : public Loop {
    private:
        std::string initializer;
        std::string update;
    
    public:
        For();
        
        void set_initializer(std::string init);
        void set_update(std::string update);

        std::string get_initializer();
        std::string get_update();
};