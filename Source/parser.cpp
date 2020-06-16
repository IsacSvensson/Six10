#include "parser.hpp"
#include "lexer.hpp"

Token* Parser::advance(){
    tokIndex++;
    if (tokIndex < tokens.size())
        return &tokens[tokIndex];
    return nullptr;
}


astNode* Parser::factor(){
    Token* tok = &tokens[tokIndex];
    
    if (tok->type == INTEGER || tok->type == FLOAT){
        advance();
        auto toReturn = new numberNode((Type)tok->type, tok);
        return (astNode*)toReturn;
    }
    return nullptr;
}

astNode* Parser::term(){
    auto left = factor();
    Token* opToken;
    astNode* right;
    binOpNode* toReturn;
    while (tokens[tokIndex].value == "*" || tokens[tokIndex].value == "/" ){
        opToken = &tokens[tokIndex];
        advance();
        right = factor();
        toReturn = new binOpNode(ARITHMETICOP, left, opToken, right);
    }
    return (astNode*)toReturn;
}

astNode* Parser::expr(){
    auto left = factor();
    Token* opToken;
    astNode* right;
    binOpNode* toReturn;

    while (tokens[tokIndex].value == "+" || tokens[tokIndex].value == "-" ){
        opToken = &tokens[tokIndex];
        advance();
        right = factor();
        toReturn = new binOpNode(ARITHMETICOP, left, opToken, right);
    }
    return (astNode*)toReturn;
}

void Parser::run(){
    auto tree = expr();
}