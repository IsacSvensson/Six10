#include "nodes.hpp"
#include "interpreter.hpp"
#include "typeCheckers.hpp"
#include <string>
#include <cmath>

RuntimeResult* Interpreter::visit(astNode* node){
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
    return toRet;
}

RuntimeResult* Interpreter::visitFloat(astNode* node){
    auto num = new Number(std::stod(((numberNode*)node)->tok.value), FLOAT);
    num->setPos(((numberNode*)node)->tok.posStart, ((numberNode*)node)->tok.posEnd);
    auto toRet = new RuntimeResult(); 
    toRet->success(num);
    return toRet;
}

RuntimeResult* Interpreter::visitBinNode(astNode* node){
    auto res = new RuntimeResult();

    auto left = res->registerResult(visit(node->left));
    auto right = res->registerResult(visit(node->right));
    if (res->error)
        return res;
    std::pair<Number*, Error*> result;

    if (((binOpNode*)node)->op->value == "+")
        result = left->addedTo(right);
    else if (((binOpNode*)node)->op->value == "-")
        result = left->subtractedBy(right);
    else if (((binOpNode*)node)->op->value == "*")
        result = left->multipliedby(right);
    else if (((binOpNode*)node)->op->value == "/")
        result = left->dividedby(right);
    else if (((binOpNode*)node)->op->value == "^")
        result = left->powedby(right);
    
    if (result.second)
        return res->failure(result.second);

    result.first->setPos(node->posStart, node->posEnd);
    res->success(result.first);

    return res;
}

RuntimeResult*  Interpreter::visitUnNode(astNode* node){
    auto res = new RuntimeResult();
    auto number = res->registerResult(visit(((UnOpNode*)node)->left));
    if (res->error)
        return res;
    auto min = new Number(-1);
    auto toRet = number->multipliedby(min);
    if (toRet.second)
        return res->failure(toRet.second);

    delete min;
    delete number;

    toRet.first->setPos(node->posStart, node->posEnd);
    res->success(toRet.first);
    return res;
}

void Number::setPos(Position* start, Position* end) {
        posStart = start;
        posEnd = end;
    }
std::pair<Number*, Error*> Number::addedTo(Number* other) {
    double val = value + other->value;
    if (this->type == FLOAT || other->type == FLOAT)
        return std::make_pair(new Number(val, FLOAT), nullptr);
    return std::make_pair(new Number(int(val)), nullptr);
}
std::pair<Number*, Error*> Number::subtractedBy(Number* other) {
    double val = value - other->value;
    if (this->type == FLOAT || other->type == FLOAT)
        return std::make_pair(new Number(val, FLOAT), nullptr);
    return std::make_pair(new Number(int(val)), nullptr);
}
std::pair<Number*, Error*> Number::multipliedby(Number* other) {
    double val = value * other->value;
    if (this->type == FLOAT || other->type == FLOAT)
        return std::make_pair(new Number(val, FLOAT), nullptr);
    return std::make_pair(new Number(int(val)), nullptr);
}
std::pair<Number*, Error*> Number::dividedby(Number* other) {
    double val = value / other->value;
    if (other->value == 0)
        return std::make_pair(nullptr, (Error*)(new RuntimeError(other->posStart->filename, *other->posStart, *other->posEnd, "Division by zero")));
    return std::make_pair(new Number(val, FLOAT), nullptr);
}
std::pair<Number*, Error*> Number::powedby(Number* other) {
    double val = pow(value, other->value);
    return std::make_pair(new Number(val, FLOAT), nullptr);
}

Number* RuntimeResult::registerResult(RuntimeResult* res){
    if (res->error)
        error = res->error;
    return res->value;
}
RuntimeResult* RuntimeResult::success(Number* value){
    this->value = value;
    this->type = value->type;
    return this;
}
RuntimeResult* RuntimeResult::failure(Error* error){
    this->error = error;
    return this;
}