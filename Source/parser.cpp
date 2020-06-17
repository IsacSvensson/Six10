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
    while (tokens[tokIndex].value == "*" || tokens[tokIndex].value == "/" ){
        opToken = &tokens[tokIndex];
        advance();
        right = factor();
        left = (astNode*)(new binOpNode(ARITHMETICOP, left, opToken, right));
    }
    return left;
}

astNode* Parser::expr(){
    auto left = term();
    Token* opToken;
    astNode* right;

    while (tokens[tokIndex].value == "+" || tokens[tokIndex].value == "-" ){
        opToken = &tokens[tokIndex];
        advance();
        right = term();
        left = (astNode*)(new binOpNode(ARITHMETICOP, left, opToken, right));
    }
    return left;
}

void Parser::run(){
    auto tree = expr();
    std::cout << tree << std::endl; 
    printTree(tree);
}

void Parser::printTree(astNode* tree){
    switch (tree->nodeType)
    {
    case ARITHMETICOP:
        std::cout << "(";
        printTree(((binOpNode*)tree)->left);
        std::cout << (((binOpNode*)tree)->op->value);
        printTree(((binOpNode*)tree)->right);
        std::cout << ")";
        break;
    case INTEGER:
        std::cout << ((numberNode*)tree)->tok.value;
        break;
    case FLOAT:
        std::cout << ((numberNode*)tree)->tok.value;
        break;
    default:
        break;
    } 
}