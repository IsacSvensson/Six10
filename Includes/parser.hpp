#pragma once 

#include "nodes.hpp"
#include "interpreter.hpp"
#include <string>


class ParseResult{
public:
    Error* error;
    astNode* node;
    astNode* registerResult(ParseResult* pr, Token* n = nullptr);
    ParseResult* success(astNode* node);
    ParseResult* failure(Error* error);
};  

class Parser{
private:
    std::vector<Token> tokens;
    std::size_t tokIndex = 0;
    Token* advance();
    ParseResult* atom();
    ParseResult* power();
    ParseResult* factor();
    ParseResult* term();
    ParseResult* expr();
public:
    Parser(std::vector<Token> tokens) : tokens(tokens) {};
    ParseResult* parse();
    void printTree(astNode* tree);
};
