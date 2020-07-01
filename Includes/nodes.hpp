#pragma once

#include "lexer.hpp"
#include "typeCheckers.hpp"
#include "interpreter.hpp"

class astNode{
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
    astNode* left;
    astNode* right;
    Position* posStart;
    Position* posEnd;
    Token tok;
    numberNode(Type nodeType, Token* token) : 
        nodeType(nodeType), left(nullptr), right(nullptr), posStart(token->posStart), posEnd(token->posEnd), tok(*token) {};
    numberNode(numberNode* node) : 
        nodeType(node->nodeType), left(node->left), right(node->right), posStart(node->posStart), posEnd(node->posEnd), tok(node->tok) {};
    Token* getToken() { return &tok; };
};

class binOpNode{
public:
    Type nodeType;
    astNode* left;
    astNode* right;
    Position* posStart;
    Position* posEnd;
    Token* op;
    binOpNode(Type nodeType, astNode* l, Token* op, astNode* r) : nodeType(nodeType), left(l), right(r), posStart(l->posStart), posEnd(r->posEnd), op(op) {};
    ~binOpNode() {delete left; delete right; delete op;};
    void doThis(){
    };
    void set(Type nodeType, astNode* l, Token* op, astNode* r);
};

class UnOpNode{
public:
    Type nodeType;
    astNode* left;
    astNode* right;
    Position* posStart;
    Position* posEnd;
    Token* op;
    UnOpNode(Type nodeType, astNode* node, Token* op) : nodeType(nodeType), left(node), posStart(op->posStart), posEnd(op->posEnd), op(op) {};
};

class VarAccessNode{
public:
    Type nodeType;
    astNode* left;
    astNode* right;
    Position* posStart;
    Position* posEnd;
    Token* varNameTok;
    VarAccessNode(Token* varNameTok) : nodeType(VARACCESSNODE), posStart(varNameTok->posStart), posEnd(varNameTok->posEnd), varNameTok(varNameTok) {};
};

class VarAssignNode{
public:
    Type nodeType;
    astNode* left;
    astNode* right;
    Position* posStart;
    Position* posEnd;
    Token* varNameTok;
    VarAssignNode(Token* varNameTok, astNode* nodeVal) : nodeType(ASSIGNMENTOP), left(nodeVal), posStart(varNameTok->posStart), posEnd(varNameTok->posEnd), varNameTok(varNameTok) {};
};

