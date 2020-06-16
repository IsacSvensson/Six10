#ifndef NODES_HPP
#define NODES_HPP

#include "typeCheckers.hpp"
#include "lexer.hpp"
#include <iostream>

class astNode{
private:
    Type nodeType;
    astNode* left;
    astNode* right;
public:
    astNode(Type nodeType, astNode* l = nullptr, astNode* r = nullptr) : 
        nodeType(nodeType), left(l), right(r) {};
};

class numberNode{
    Type nodeType;
    Token tok;
public:
    numberNode(Type nodeType, Token* token) : 
        nodeType(nodeType), tok(*token) {};
    Token* getToken() { return &tok; };
};

class binOpNode{
    Type nodeType;
    astNode* left;
    astNode* right;
    Token* op;
public:
    binOpNode(Type nodeType, astNode* l, Token* op, astNode* r) : nodeType(nodeType), left(l), right(r), op(op) {};
    ~binOpNode() {delete left; delete right; delete op;};
    void doThis(){
    };
    void set(Type nodeType, astNode* l, Token* op, astNode* r);
};

class UnOpNode{
    Type nodeType;
    astNode* left;
    Token* op;
public:
    UnOpNode(Type nodeType, astNode* l, Token* op) : nodeType(nodeType), left(l), op(op) {};
    ~UnOpNode() {delete left; delete op;};
    void doThis() {std::cout << ((numberNode*)(left))->getToken()->value << op->value; };
};

#endif