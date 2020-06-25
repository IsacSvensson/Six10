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

RuntimeResult* Interpreter::visitInteger(astNode* node){
    auto num = new Number(std::stoi(((numberNode*)node)->tok.value));
    num->setPos(((numberNode*)node)->tok.posStart, ((numberNode*)node)->tok.posEnd);
    auto toRet = new RuntimeResult(); 
    toRet->success(num);
    delete num;
    return toRet;
}

RuntimeResult* Interpreter::visitFloat(astNode* node){
    auto num = new Number(std::stod(((numberNode*)node)->tok.value), FLOAT);
    num->setPos(((numberNode*)node)->tok.posStart, ((numberNode*)node)->tok.posEnd);
    auto toRet = new RuntimeResult(); 
    toRet->success(num);
    delete num;
    return toRet;
}

RuntimeResult* Interpreter::visitBinNode(astNode* node){
    auto res = new RuntimeResult();

    auto left = res->registerResult(visit(node->left));
    auto right = res->registerResult(visit(node->right));
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

RuntimeResult*  Interpreter::visitUnNode(astNode* node){
    auto number = visit(((UnOpNode*)node)->node);
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
        if (this->type == FLOAT || other->type == FLOAT)
            return new Number(val, FLOAT);
        return new Number(int(val));
    }
    Number* Number::subtractedBy(Number* other) {
        double val = value - other->value;
        if (this->type == FLOAT || other->type == FLOAT)
            return new Number(val, FLOAT);
        return new Number(int(val));
    }
    Number* Number::multipliedby(Number* other) {
        double val = value * other->value;
        if (this->type == FLOAT || other->type == FLOAT)
            return new Number(val, FLOAT);
        return new Number(int(val));
    }
    Number* Number::dividedby(Number* other) {
        double val = value / other->value;
        return new Number(val, FLOAT);
    }

Number* RuntimeResult::registerResult(RuntimeResult* res){
    if (res->error)
        return res->value
    return res;
}
RuntimeResult* RuntimeResult::success(Number* value){
    this->value = new Number(value);
    return this;
}
RuntimeResult* RuntimeResult::failure(Error* error){
    this->error = error;
    return this;
}