/*
Contains exceptions
*/

#include <exception>
#include <string>

class CustomException : public std::runtime_error {
    protected:
        int code;

        CustomException(int code, std::string msg);
        const char* what() const noexcept;
};

class VariableException : public CustomException {
    private:
        static inline char* errors[] = {
            "invalid value provided",
            "invalid index provided",
            "variable is not an array",
        };
        int line;

    public:
        VariableException(const char* message);
        VariableException(const std::string message);
        VariableException(std::size_t index);

        VariableException(const char* message, std::size_t line);
        VariableException(const std::string message, std::size_t line);
        VariableException(std::size_t index, std::size_t line);

        const char* what() const noexcept;
};

class ProgramException : public CustomException {
    private:
        static inline char* errors[] = {
            "invalid file provided",
        };

    public:
        ProgramException(const char* message);
        ProgramException(const std::string message);
        ProgramException(std::size_t index);
};

class SyntaxException : public CustomException {
    private:
        static inline char* errors[] = {
            "FINE mancante",
            "Questa riga deve contenere solo una parola chiave",
            "Questa riga non puo' contenere solo la parola chiave",
        };
        int line;

    public:
        SyntaxException(const char* message);
        SyntaxException(const std::string message);
        SyntaxException(std::size_t index);

        SyntaxException(const char* message, std::size_t line);
        SyntaxException(const std::string message, std::size_t line);
        SyntaxException(std::size_t index, std::size_t line);

        const char* what() const noexcept;
};