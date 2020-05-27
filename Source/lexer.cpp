#include "lexer.hpp"

enum types {
    LETTER,
    NUMBER,
    IDENTIFIER,
    OPERATOR,
    NEWLINE,
    PARENTHESES,
    TABB,
    COMMENT,
    KEYWORD,
    DOT
};

std::vector<std::pair<int, std::string>> lexer(std::string sourceCode){
    return;
}