#pragma once 

#include "nodes.hpp"
#include "interpreter.hpp"
#include <string>


class ParseResult{
public:
    Error* error;
    astNode* node;
    int advanceCount;
    ParseResult() : error(nullptr), node(nullptr), advanceCount(0) {};
    astNode* registerResult(ParseResult* res);
    void registerAdvancement();
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
    ParseResult* arithmExpr();
    ParseResult* compExpr();
    ParseResult* expr();
    ParseResult* ifExpr();
public:
    Parser(std::vector<Token> tokens) : tokens(tokens) {};
    ParseResult* parse();
    void printTree(astNode* tree);
};
