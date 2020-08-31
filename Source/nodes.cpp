#include<iostream>
#include <sstream>
#include "nodes.hpp"
#include "parser.hpp"
#include "helpers.hpp"


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

Position::Position(){
};

void Position::advance(char currentChar){
    index++;
    col++;
    if (currentChar == '\n'){
        line++;
        col = 0;
    }
}

std::string Position::getPos(){
    std::stringstream ss;
    ss << "Line: " << line+1 << ", Col:" << col+1;

    std::string pos;
    pos = ss.str();
    return pos;
}

void Position::set(std::string fn, std::string ftxt){
    index =0;
    line = 0;
    col = 0;
    filename = fn;
    filetext = ftxt;
}