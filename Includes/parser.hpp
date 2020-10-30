#pragma once 

#include "nodes.hpp"
#include "interpreter.hpp"
#include <string>


class ParseResult{
public:
    Error* error;
    astNode* node;
    int advanceCount;
    int lastRegisteredAdvanceCount;
    int toReverseCount;
    ParseResult() : error(nullptr), node(nullptr), advanceCount(0), lastRegisteredAdvanceCount(0), toReverseCount(0) {};
    astNode* registerResult(ParseResult* res);
    astNode* tryRegister(ParseResult* res);
    void registerAdvancement();
    ParseResult* success(astNode* node);
    ParseResult* failure(Error* error);
};  

class Parser{
private:
    std::vector<Token> tokens;
    std::size_t tokIndex;
    Token* advance();
    ParseResult* atom();
    ParseResult* call();
    ParseResult* power();
    ParseResult* factor();
    ParseResult* term();
    ParseResult* arithmExpr();
    ParseResult* compExpr();
    ParseResult* expr();
    ParseResult* listExpr();
    ParseResult* ifExpr();
    ParseResult* forExpr();
    ParseResult* whileExpr();
    ParseResult* funcDef();
    ParseResult* statements();
    Token* reverse(int amount = 1);
public:
    Parser() : tokIndex(0) {};
    Parser(std::vector<Token> tokens) : tokens(tokens), tokIndex(0) {};
    ParseResult* parse();
    void reset(std::vector<Token> tokens);
};
