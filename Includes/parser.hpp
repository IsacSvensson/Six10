#ifndef PARSER_HPP
#define PARSER_HPP

#include "nodes.hpp"
#include <string>

class Parser{
private:
    std::vector<Token> tokens;
    std::size_t tokIndex = 0;
    Token* advance();
    astNode* factor();
    astNode* term();
    astNode* expr();
public:
    Parser(std::vector<Token> tokens) : tokens(tokens) {};
    void run();
};

#endif