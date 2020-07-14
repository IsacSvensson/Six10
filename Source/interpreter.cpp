#include "nodes.hpp"
#include "interpreter.hpp"
#include "helpers.hpp"
#include <string>
#include <cmath>
#include <utility>
#include <sstream>
#include <iostream>

RuntimeResult* Interpreter::visit(astNode* node, Context* context){
    switch (node->nodeType)
    {
    case INTEGER:
        return visitInteger(node, context);
        break;
    case FLOAT:
        return visitFloat(node, context);
        break;
    case ARITHMETICOP:
        return visitBinNode(node, context);
        break;
    case UNARYOP:
        return visitUnNode(node, context);
        break;
    case ASSIGNMENTOP:
        return visitVarAssignNode(node, context);
        break;
    case VARACCESSNODE:
        return visitVarAccessNode(node, context);
        break;
    default:
        break;
    }
    return nullptr;
}

RuntimeResult* Interpreter::visitInteger(astNode* node, Context* context){
    auto num = new Number(std::stoi(((numberNode*)node)->tok.value));
    num->setPos(((numberNode*)node)->tok.posStart, ((numberNode*)node)->tok.posEnd);
    num->setContext(context);
    auto toRet = new RuntimeResult(); 
    toRet->success(num);
    return toRet;
}

RuntimeResult* Interpreter::visitFloat(astNode* node, Context* context){
    auto num = new Number(std::stod(((numberNode*)node)->tok.value), FLOAT);
    num->setPos(((numberNode*)node)->tok.posStart, ((numberNode*)node)->tok.posEnd);
    num->setContext(context);
    auto toRet = new RuntimeResult(); 
    toRet->success(num);
    return toRet;
}

RuntimeResult* Interpreter::visitBinNode(astNode* node, Context* context){
    auto res = new RuntimeResult();

    auto left = res->registerResult(visit(node->left, context));
    auto right = res->registerResult(visit(node->right, context));
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
    else if (((binOpNode*)node)->op->value == "==")
        result = left->getComparisonEQ(right);
    else if (((binOpNode*)node)->op->value == "!=")
        result = left->getComparisonNotEq(right);
    else if (((binOpNode*)node)->op->value == "<")
        result = left->getComparisonLT(right);
    else if (((binOpNode*)node)->op->value == ">")
        result = left->getComparisonGT(right);
    else if (((binOpNode*)node)->op->value == "<=")
        result = left->getComparisonLTE(right);
    else if (((binOpNode*)node)->op->value == ">=")
        result = left->getComparisonGTE(right);
    else if (((binOpNode*)node)->op->value == "and" || ((binOpNode*)node)->op->value == "&&")
        result = left->logicalAnd(right);
    else if (((binOpNode*)node)->op->value == "or" || ((binOpNode*)node)->op->value == "||")
        result = left->logicalOr(right);
    if (result.second)
        return res->failure(result.second);

    result.first->setPos(node->posStart, node->posEnd);
    res->success(result.first);

    return res;
}

RuntimeResult*  Interpreter::visitUnNode(astNode* node, Context* context){
    auto res = new RuntimeResult();
    std::pair<Number*, Error*> toRet;
    auto number = res->registerResult(visit(((UnOpNode*)node)->left, context));
    if (res->error)
        return res;
    if (((UnOpNode*)node)->op->value.compare("not"))
    {
        toRet = number->logicalNot();
        if (toRet.second)
            return res->failure(toRet.second);
        delete number;
        return res->success(toRet.first);
    }
    else if (((UnOpNode*)node)->op->value == "-"){
        auto min = new Number(-1);
        toRet = number->multipliedby(min);
        if (toRet.second)
            return res->failure(toRet.second);

        delete min;
        delete number;

        toRet.first->setPos(node->posStart, node->posEnd);
        res->success(toRet.first);
        return res;
    }
    return res->failure((Error*)new RuntimeError(node->posStart->filename, *node->posStart, *node->posEnd, "Could not complete operation."));
}


RuntimeResult* Interpreter::visitVarAccessNode(astNode* node, Context* context){
    auto res = new RuntimeResult();
    auto name = ((VarAccessNode*)node)->varNameTok->value;
    auto value = new Number(context->symTab->get(name));
    value->setPos(node->posStart, node->posEnd);

    if (!value)
        res->failure((Error*)new RuntimeError(node->posEnd->filename ,*node->posStart, *node->posEnd, ("'" + name + "' is not defined"), context));
    
    return res->success(value);
}

RuntimeResult* Interpreter::visitVarAssignNode(astNode* node, Context* context){
    auto res = new RuntimeResult();
    auto name = ((VarAssignNode*)node)->varNameTok->value;
    auto value = res->registerResult(visit(node->left, context));

    if (res->error)
        return res;
    
    context->symTab->set(name, value);
    return res->success(value);
}

Number* Number::setPos(Position* start, Position* end) {
        posStart = start;
        posEnd = end;
        return this;
    }
std::pair<Number*, Error*> Number::addedTo(Number* other) {
    double val = value + other->value;
    if (this->type == FLOAT || other->type == FLOAT)
        return std::make_pair((new Number(val, FLOAT))->setContext(this->context), nullptr);
    return std::make_pair((new Number(int(val)))->setContext(this->context), nullptr);
}
std::pair<Number*, Error*> Number::subtractedBy(Number* other) {
    double val = value - other->value;
    if (this->type == FLOAT || other->type == FLOAT)
        return std::make_pair((new Number(val, FLOAT))->setContext(this->context), nullptr);
    return std::make_pair((new Number(int(val)))->setContext(this->context), nullptr);
}
std::pair<Number*, Error*> Number::multipliedby(Number* other) {
    double val = value * other->value;
    if (this->type == FLOAT || other->type == FLOAT)
        return std::make_pair((new Number(val, FLOAT))->setContext(this->context), nullptr);
    return std::make_pair((new Number(int(val)))->setContext(this->context), nullptr);
}
std::pair<Number*, Error*> Number::dividedby(Number* other) {
    double val = value / other->value;
    if (other->value == 0)
        return std::make_pair(nullptr, (Error*)(new RuntimeError(other->posStart->filename, *other->posStart, *other->posEnd, "Division by zero", context)));
    return std::make_pair((new Number(val, FLOAT))->setContext(this->context), nullptr);
}
std::pair<Number*, Error*> Number::powedby(Number* other) {
    double val = pow(value, other->value);
    return std::make_pair((new Number(val, FLOAT))->setContext(this->context), nullptr);
}

Number* Number::setContext(Context* context = nullptr){
    this->context = context;
    return this;
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

unsigned int SymbolTable::symHash(std::string id){
    unsigned int val = 0;
    short i = 1;
    for (char& c: id){
        val += pow(c, i);
        i++;
    }
    return val%symtab.size();
}

void SymbolTable::set(std::string id, Number* val){
    std::size_t index = symHash(id);
    auto atIndex = symtab[index];
    if (atIndex){
        while (atIndex->name != id && atIndex->next)
            atIndex = atIndex->next;
        if (atIndex->name == id){
            atIndex->value = val;
        }
        else if (parent)
            parent->set(id, val);
        else{
            atIndex->next = new SymNode(id, val);
            numOfSyms++;
        }
    }
    else{
        symtab[index] = new SymNode(id, val);
    }
}

Number* SymbolTable::get(std::string id){
    std::size_t index = symHash(id);
    auto atIndex = symtab[index];
    if (atIndex){
        while (atIndex->name != id && atIndex->next)
            atIndex = atIndex->next;
        if (atIndex->name == id){
            return &atIndex->value;
        }
        else if (parent)
            return parent->get(id);
    }
    return nullptr;
}

std::pair<std::vector<Token>, Error*> Lexer::makeTokens(){
    while (it < sourceCode.end())
    {
        auto token = isType(it, sourceCode.end());
        if (token.first == SPACE){
            advance();
            continue;
            }
        else if (token.first == INVALID){
            Position posStart = pos;
            auto ch = std::string(1, *it);
            advance();
            return std::make_pair(tokens, (Error*) new IllegalCharError(filename, ch, posStart, Position(pos)));
            }
        tokens.push_back(Token(token, it, &pos));
        for(int i = 0; i < token.second; i++)
            advance();
    }
    tokens.push_back(Token(EOF_, std::string(), &pos, &pos));
    return std::make_pair(tokens, nullptr);
}

void Lexer::advance(){
    char toSend = ' ';
    if (!tokens.empty())
        if (tokens.back().type == EOL)
            toSend = '\n';
    pos.advance(toSend);
    it++;
}



std::string Error::toString(){
    std::string toReturn;
    toReturn = errorName + ": '" + details + "'\nFile: \"" + fileName + "\": " + posStart.getPos();
    toReturn += stringWithArrows(posStart.filetext, &posStart, &posEnd);
    return toReturn;
}

std::string RuntimeError::toString(){
    std::string result = "";
    result += generateTraceback();
    result += errorName + ": " + details + "\n";
    result += stringWithArrows(posStart.filetext, &posStart, &posEnd);
    return result;
}

std::string RuntimeError::generateTraceback(){
    std::string result = "";
    std::string temp;
    auto pos = &posStart;
    auto ctx = context;

    std::stringstream ss;

    while (ctx){
        ss << "File " << pos->filename << ", line " << (pos->line + 1) << ", in " << ctx->displayName;
        std::getline(ss, temp);
        result = temp + "\n" +  result;
        pos = ctx->parentEntryPos;
        ctx = ctx->parent;
    }

    return "Traceback (most recent call last):\n" + result;
}

std::pair<Number*, Error*> Number::getComparisonEQ(Number* other){
    double val = value == other->value;
    return std::make_pair((new Number(val, INTEGER))->setContext(this->context), nullptr);
}
std::pair<Number*, Error*> Number::getComparisonNotEq(Number* other){
    double val = value != other->value;
    return std::make_pair((new Number(val, INTEGER))->setContext(this->context), nullptr);
}
std::pair<Number*, Error*> Number::getComparisonLT(Number* other){
    double val = value < other->value;
    return std::make_pair((new Number(val, INTEGER))->setContext(this->context), nullptr);
}
std::pair<Number*, Error*> Number::getComparisonGT(Number* other){
    double val = value > other->value;
    return std::make_pair((new Number(val, INTEGER))->setContext(this->context), nullptr);
}
std::pair<Number*, Error*> Number::getComparisonLTE(Number* other){
    double val = value <= other->value;
    return std::make_pair((new Number(val, INTEGER))->setContext(this->context), nullptr);
}
std::pair<Number*, Error*> Number::getComparisonGTE(Number* other){
    double val = value >= other->value;
    return std::make_pair((new Number(val, INTEGER))->setContext(this->context), nullptr);
}
std::pair<Number*, Error*> Number::logicalAnd(Number* other){
    double val = value && other->value;
    return std::make_pair((new Number(val, INTEGER))->setContext(this->context), nullptr);
}
std::pair<Number*, Error*> Number::logicalOr(Number* other){
    double val = value || other->value;
    return std::make_pair((new Number(val, INTEGER))->setContext(this->context), nullptr);
}
std::pair<Number*, Error*> Number::logicalNot(){
    double val = !value;
    return std::make_pair((new Number(val, INTEGER))->setContext(this->context), nullptr);
}