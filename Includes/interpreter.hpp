#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "nodes.hpp"
#include "position.hpp"
#include "lexer.hpp"

class RuntimeResult;
class astNode;

class Number{
public:
    Type type;
    double value;
    Position* posStart;
    Position* posEnd;
    Number(double val, Type t = INTEGER) : type(t), value(val) {setPos();};
    Number(Number* num) : type(num->type), value(num->value), posStart(new Position(*num->posStart)), posEnd(new Position(*num->posEnd)) {};
    void setPos(Position* start = nullptr, Position* end = nullptr);
    std::pair<Number*, Error*> addedTo(Number* other);
    std::pair<Number*, Error*> subtractedBy(Number* other);
    std::pair<Number*, Error*> multipliedby(Number* other);
    std::pair<Number*, Error*> dividedby(Number* other);
    std::pair<Number*, Error*> powedby(Number* other);
};

class Interpreter{
public:
    astNode* node;
    RuntimeResult* visit(astNode* node);
    RuntimeResult* visitInteger(astNode* node);
    RuntimeResult* visitFloat(astNode* node);
    RuntimeResult* visitBinNode(astNode* node);
    RuntimeResult* visitUnNode(astNode* node);
    Interpreter(astNode* n) : node(n) {};
};

class RuntimeResult{
public:
    Type type;
    Number* value;
    Error* error;
    RuntimeResult() : type(INVALID), value(nullptr), error(nullptr) {};
    Number* registerResult(RuntimeResult* value);
    RuntimeResult* success(Number* value);
    RuntimeResult* failure(Error* error);
};

#endif