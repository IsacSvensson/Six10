#include "interpreter.hpp"
#include "typeCheckers.hpp"

void Interpreter::visit(astNode* node){
    switch (node->nodeType)
    {
    case INTEGER:
        visitInteger(node);
        break;
    case FLOAT:
        visitFloat(node);
        break;
    case ARITHMETICOP:
        visitBinNode(node);
        visit(node->left);
        visit(node->right);
        break;
    case UNARYOP:
        visitUnNode(node);
        visit(((UnOpNode*)node)->node);
        break;
    default:
        break;
    }
}

void Interpreter::visitInteger(astNode* node){
    std::cout << "Found Integer Node!" << std::endl;
    if(node->nodeType == STRING)
        std::cout << "hejd";
}

void Interpreter::visitFloat(astNode* node){
    std::cout << "Found Float Node!" << std::endl;
    if(node->nodeType == STRING)
        std::cout << "hejd";
}

void Interpreter::visitBinNode(astNode* node){
    std::cout << "Found Binary Operation Node!" << std::endl;
    if(node->nodeType == STRING)
        std::cout << "hejd";
}

void Interpreter::visitUnNode(astNode* node){
    std::cout << "Found Unary Operation Node!" << std::endl;
    if(node->nodeType == STRING)
        std::cout << "hejd";
}
