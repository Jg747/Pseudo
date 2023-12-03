#pragma once

#include <string>
#include <vector>

#define WHITESPACES " \t\r\n"

enum program_mode_e {
    INTERPRETER,
    CPP_TO_PSEUDO,
    PSEUDO_TO_CPP
};

class ProgramMode {
    protected:
        std::vector<std::string> content;
        
    public:
        ProgramMode();
        ProgramMode(std::vector<std::string>& content);
        virtual void run();
};

class Program {
    private:
        enum program_mode_e mode;
        std::string in_file;
        std::vector<std::string> content;

        ProgramMode *run;

    public:
        Program();
        Program(enum program_mode_e mode);
        Program(enum program_mode_e mode, std::string in_file);
        ~Program();

        void set_input_file(std::string input);
        std::string get_input_file();

        static std::string trim_string(std::string str);
        static std::vector<std::string> tokenize_string(std::string str);
        static std::vector<std::string> split_string(std::string str, std::string regex);

        void read_file();
        void run_program();
};