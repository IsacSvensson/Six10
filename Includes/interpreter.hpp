#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "nodes.hpp"
#include "position.hpp"
#include "lexer.hpp"


class Number{
public:
    Type type;
    double value;
    Position* posStart;
    Position* posEnd;
    Number(double val, Type t = INTEGER) : type(t), value(val) {setPos();};
    void setPos(Position* start = nullptr, Position* end = nullptr);
    Number* addedTo(Number* other);
    Number* subtractedBy(Number* other);
    Number* multipliedby(Number* other);
    Number* dividedby(Number* other);
};

class Interpreter{
public:
    astNode* node;
    Number* visit(astNode* node);
    Number* visitInteger(astNode* node);
    Number* visitFloat(astNode* node);
    Number* visitBinNode(astNode* node);
    Number* visitUnNode(astNode* node);
    Interpreter(astNode* n) : node(n) {};
};

#endif