#include "interpreter.hpp"
#include "typeCheckers.hpp"
#include "nodes.hpp"
#include <string>

Number* Interpreter::visit(astNode* node){
    switch (node->nodeType)
    {
    case INTEGER:
        return visitInteger(node);
        break;
    case FLOAT:
        return visitFloat(node);
        break;
    case ARITHMETICOP:
        return visitBinNode(node);
        break;
    case UNARYOP:
        return visitUnNode(node);
        break;
    default:
        break;
    }
    return nullptr;
}

Number* Interpreter::visitInteger(astNode* node){
    auto toRet = new Number(std::stoi(((numberNode*)node)->tok.value));
    toRet->setPos(((numberNode*)node)->tok.posStart, ((numberNode*)node)->tok.posEnd);
    return toRet; 
}

Number*  Interpreter::visitFloat(astNode* node){
    auto toRet = new Number(std::stod(((numberNode*)node)->tok.value), false);
    toRet->setPos(((numberNode*)node)->tok.posStart, ((numberNode*)node)->tok.posEnd);
    return toRet;
}

Number*  Interpreter::visitBinNode(astNode* node){
    auto left = visit(node->left);
    auto right = visit(node->right);
    Number* result;

    if (((binOpNode*)node)->op->value == "+")
        result = left->addedTo(right);
    else if (((binOpNode*)node)->op->value == "-")
        result = left->subtractedBy(right);
    else if (((binOpNode*)node)->op->value == "*")
        result = left->multipliedby(right);
    else if (((binOpNode*)node)->op->value == "/")
        result = left->dividedby(right);
    
    delete left;
    delete right;

    result->setPos(node->posStart, node->posEnd);
    return result;
}

Number*  Interpreter::visitUnNode(astNode* node){
    auto number = visit(node);
    auto min = new Number(-1);
    auto toRet = number->multipliedby(min);

    delete min;
    delete number;

    toRet->setPos(node->posStart, node->posEnd);
    return toRet;
}

void Number::setPos(Position* start, Position* end) {
        posStart = start;
        posEnd = end;
    }
    Number* Number::addedTo(Number* other) {
        double val = value + other->value;
        if (this->integer == false || other->integer == false)
            return new Number(val, false);
        return new Number(int(val));
    }
    Number* Number::subtractedBy(Number* other) {
        double val = value - other->value;
        if (this->integer == false || other->integer == false)
            return new Number(val, false);
        return new Number(int(val));
    }
    Number* Number::multipliedby(Number* other) {
        double val = value * other->value;
        if (this->integer == false || other->integer == false)
            return new Number(val, false);
        return new Number(int(val));
    }
    Number* Number::dividedby(Number* other) {
        double val = value / other->value;
        return new Number(val, false);
    }