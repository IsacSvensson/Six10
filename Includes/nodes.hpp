#ifndef NODES_HPP
#define NODES_HPP

#include "typeCheckers.hpp"
#include "lexer.hpp"
#include "interpreter.hpp"
#include <iostream>

class astNode{
private:
public:
    Type nodeType;
    astNode* left;
    astNode* right;
    Position* posStart;
    Position* posEnd;
    astNode(Type nodeType, astNode* l = nullptr, astNode* r = nullptr) : 
        nodeType(nodeType), left(l), right(r), posStart(nullptr), posEnd(nullptr) {};
    friend std::ostream& operator<<(std::ostream& os , const astNode* node);
};

class numberNode{
public:
    Type nodeType;
    Token tok;
    Position* posStart;
    Position* posEnd;
    numberNode(Type nodeType, Token* token) : 
        nodeType(nodeType), tok(*token), posStart(tok.posStart), posEnd(tok.posEnd) {};
    Token* getToken() { return &tok; };
};

class binOpNode{
public:
    Type nodeType;
    astNode* left;
    astNode* right;
    Token* op;
    Position* posStart;
    Position* posEnd;
    binOpNode(Type nodeType, astNode* l, Token* op, astNode* r) : nodeType(nodeType), left(l), right(r), op(op), posStart(op->posStart), posEnd(op->posEnd) {};
    ~binOpNode() {delete left; delete right; delete op;};
    void doThis(){
    };
    void set(Type nodeType, astNode* l, Token* op, astNode* r);
};

class UnOpNode{
public:
    Type nodeType;
    astNode* node;
    Token* op;
    Position* posStart;
    Position* posEnd;
    UnOpNode(Type nodeType, astNode* node, Token* op) : nodeType(nodeType), node(node), op(op), posStart(op->posStart), posEnd(op->posEnd) {};
    ~UnOpNode() {delete node; delete op;};
};

#endif