#ifndef NODES_HPP
#define NODES_HPP

#include "typeCheckers.hpp"
#include "lexer.hpp"
#include <iostream>

class astNode{
private:
public:
    Type nodeType;
    astNode* left;
    astNode* right;
    astNode(Type nodeType, astNode* l = nullptr, astNode* r = nullptr) : 
        nodeType(nodeType), left(l), right(r) {};
    friend std::ostream& operator<<(std::ostream& os , const astNode* node);
};

class numberNode{
public:
    Type nodeType;
    Token tok;
    numberNode(Type nodeType, Token* token) : 
        nodeType(nodeType), tok(*token) {};
    Token* getToken() { return &tok; };
};

class binOpNode{
public:
    Type nodeType;
    astNode* left;
    astNode* right;
    Token* op;
    binOpNode(Type nodeType, astNode* l, Token* op, astNode* r) : nodeType(nodeType), left(l), right(r), op(op) {};
    ~binOpNode() {delete left; delete right; delete op;};
    void doThis(){
    };
    void set(Type nodeType, astNode* l, Token* op, astNode* r);
};

class UnOpNode{
public:
    Type nodeType;
    astNode* left;
    Token* op;
    UnOpNode(Type nodeType, astNode* l, Token* op) : nodeType(nodeType), left(l), op(op) {};
    ~UnOpNode() {delete left; delete op;};
    void doThis() {std::cout << ((numberNode*)(left))->getToken()->value << op->value; };
};

#endif