#include "parser.hpp"
#include "lexer.hpp"

Token Parser::advance(){
    tokIndex++;
    if (tokIndex < tokens.size())
        return tokens[tokIndex];
}

numberNode Parser::factor(){
    Token* tok = &tokens[tokIndex];
    
    if (tok->type == INTEGER || tok->type == FLOAT){
        advance();
        return numberNode(tok);
    }
}