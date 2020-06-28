#include<iostream>
#include "nodes.hpp"
#include "parser.hpp"
#include "lexer.hpp"
#include "typeCheckers.hpp"

std::ostream& operator<<(std::ostream& os , const astNode* node){
    std::string type;
    switch (node->nodeType)
    {
    case INTEGER: case FLOAT:
        type = getType(((numberNode*)node)->tok);
        return os << "(" << type << ":" << 
            ((numberNode*)node)->tok.value << ")";
        break;
    case ARITHMETICOP:
        type = getType(*((binOpNode*)node)->op);
        return os << "(" << type << ":" << 
            ((binOpNode*)node)->left << ((binOpNode*)node)->op->value << ((binOpNode*)node)->right << ")";
        break;
    case UNARYOP:
        type = getType(*((UnOpNode*)node)->op);
        return os << "(" << type << ":" << 
            ((UnOpNode*)node)->op->value << ((numberNode*)(((UnOpNode*)node)->left))->tok.value << ")";
        break;    
    default:
        break;
    }
    return os << getType(((numberNode*)node)->tok);
}