#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "nodes.hpp"

class Interpreter{ 
public:
    astNode* node;
    void visit(astNode* node);
    void visitInteger(astNode* node);
    void visitFloat(astNode* node);
    void visitBinNode(astNode* node);
    void visitUnNode(astNode* node);
    Interpreter(astNode* n) : node(n) {};
};

#endif