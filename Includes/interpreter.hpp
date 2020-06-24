#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "nodes.hpp"
#include "position.hpp"


class Number{
public:
    Number(double val, bool integer = true) : integer(integer), value(val) {setPos();};
    bool integer;
    double value;
    Position* posStart;
    Position* posEnd;
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