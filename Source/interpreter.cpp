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
    case IFSTATMENT:
        return visitIfNode(node, context);
        break;
    case FORLOOP:
        return visitForNode(node, context);
        break;
    case WHILELOOP:
        return visitWhileNode(node, context);
        break;
    case FUNCDEF:
        return visitFuncDefNode(node, context);
        break;
    case FUNCCALL:
        return visitCallNode(node, context);
        break;
    case STRING:
        return visitString(node, context);
        break;
    case LIST:
        return visitListNode(node, context);
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

RuntimeResult* Interpreter::visitString(astNode* node, Context* context){
    auto val = new String(((StringNode*)node)->tok.value);
    val->setPos(((StringNode*)node)->tok.posStart, ((StringNode*)node)->tok.posEnd);
    val->setContext(context);
    auto toRet = new RuntimeResult(); 
    toRet->success(val);
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
    std::pair<Value*, Error*> result;

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
    std::pair<Value*, Error*> toRet;
    auto number = res->registerResult(visit(((UnOpNode*)node)->left, context));
    if (res->error)
        return res;
    if (((UnOpNode*)node)->op->value == "not" || ((UnOpNode*)node)->op->value == "!")
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
    auto content = context->symTab->get(name);
    Value* value = nullptr;
    if (content && (content->type == INTEGER || content->type == FLOAT)){
        value = new Number((Number*)content);
        value->setPos(node->posStart, node->posEnd);
    }

    if (content && (content->type == FUNCDEF || content->type == LIST)){
        value = copyValue(content);
        value->setPos((node)->posStart, (node)->posEnd);
    }

    if (!value)
        return res->failure((Error*)new RuntimeError(node->posEnd->filename ,*node->posStart, *node->posEnd, ("'" + name + "' is not defined"), context));
    
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

RuntimeResult* Interpreter::visitIfNode(astNode* node, Context* context){
    auto res = new RuntimeResult();
    
    for (std::pair<astNode*, astNode*>& ifCase : ((IfNode*)node)->cases){
        auto conditionValue = res->registerResult(visit(ifCase.first, context));
        if (res->error) return res;

        if (conditionValue->isTrue()){
            auto exprValue = res->registerResult(visit(ifCase.second, context));
            if (res->error) return res;
            return res->success(exprValue);
        }
    }

    if (((IfNode*)node)->elseCase){
        auto elseValue = res->registerResult(visit(((IfNode*)node)->elseCase, context));
        if (res->error) return res;
        return res->success(elseValue);
    }
    return res->success(nullptr);
}

RuntimeResult* Interpreter::visitForNode(astNode* node, Context* context){
    auto res = new RuntimeResult();
    std::vector<Value*> elements;

    auto startVal = (Number*)res->registerResult(visit((((ForNode*)node)->startValueNode), context));
    if (res->error) return res;
    auto endVal = (Number*)res->registerResult(visit((((ForNode*)node)->endValueNode), context));
    if (res->error) return res;

    Number* stepVal = nullptr;
    if (((ForNode*)node)->stepValueNode){
        stepVal = (Number*)res->registerResult(visit((((ForNode*)node)->stepValueNode), context));
        if (res->error) return res;
    }
    else
        stepVal = new Number(1);
    
    double i = startVal->value;

    if (stepVal->value <= 0)
        while (i > endVal->value){
            context->symTab->set(((ForNode*)node)->varNameTok.value, new Number(i));
            i += stepVal->value;

            elements.push_back(res->registerResult(visit(((ForNode*)node)->bodyNode, context)));
            if (res->error) return res;
        }
    else
        while (i < endVal->value){
            context->symTab->set(((ForNode*)node)->varNameTok.value, new Number(i));
            i += stepVal->value;

            elements.push_back(res->registerResult(visit(((ForNode*)node)->bodyNode, context)));
            if (res->error) return res;
        }
    return res->success((Value*)(new List(elements))->setContext(context)->setPos(((ForNode*)node)->posStart, ((ForNode*)node)->posEnd));
}

RuntimeResult* Interpreter::visitWhileNode(astNode* node, Context* context){
    auto res = new RuntimeResult();
    std::vector<Value*> elements;

    while (true)
    {
        auto condition = res->registerResult(visit(((WhileNode*)node)->conditionNode, context));
        if (!condition->isTrue()) break;

        elements.push_back(res->registerResult(visit(((WhileNode*)node)->bodyNode, context)));
        if (res->error) return res;
    }
    return res->success((Value*)(new List(elements))->setContext(context)->setPos(((ForNode*)node)->posStart, ((ForNode*)node)->posEnd));
}

RuntimeResult* Interpreter::visitFuncDefNode(astNode* node, Context* context){
    auto res = new RuntimeResult();
    std::string funcName = ((FuncDefNode*)node)->varNameTok->value;
    astNode* bodyNode = ((FuncDefNode*)node)->bodyNode;
    std::vector<std::string> argNames;

    for (int i = 0; i < ((FuncDefNode*)node)->argNameToks.size(); i++){
        argNames.push_back(((FuncDefNode*)node)->argNameToks[i].value);
    }
    Function* funcValue = new Function(funcName, bodyNode, argNames);
    funcValue->setContext(context)->setPos(node->posStart, node->posEnd);

    if (((FuncDefNode*)node)->varNameTok)
        context->symTab->set(funcName, funcValue);

    return res->success(funcValue);
}

RuntimeResult* Interpreter::visitCallNode(astNode* node, Context* context){
    auto res = new RuntimeResult();
    std::vector<Value*> args;
    
    auto valueToCall = res->registerResult(visit(((CallNode*)node)->nodeToCall, context));
    if (res->error) return res;

    valueToCall = copyValue(valueToCall)->setPos(node->posStart, node->posEnd);

    for (int i = 0; i < ((CallNode*)node)->argNodes.size(); i++){
        args.push_back(res->registerResult(visit(((CallNode*)node)->argNodes[i], context)));
        if (res->error) return res;
    }

    auto returnVal = res->registerResult(((Function*)valueToCall)->execute(args));
    if (res->error) return res;

    return res->success(returnVal);
}

RuntimeResult* Interpreter::visitListNode(astNode* node, Context* context){
    auto res = new RuntimeResult();
    std::vector<Value*> elements;

    for (auto &element : ((ListNode*)node)->elementNodes){
        elements.push_back(res->registerResult(visit(element, context)));
        if (res->error) return res;
    }
    return res->success((Value*)(new List(elements))->setContext(context)->setPos(((ListNode*)node)->posStart, ((ListNode*)node)->posEnd));
}

Value* Value::setPos(Position* start, Position* end) {
        posStart = start;
        posEnd = end;
        return this;
    }
std::pair<Value*, Error*> Value::addedTo(Value* other) {
    if ((other->type == INTEGER || other->type == FLOAT)&&(this->type == INTEGER || this->type == FLOAT)){
        double val = ((Number*)this)->value + ((Number*)other)->value;
        if (this->type == FLOAT || other->type == FLOAT)
            return std::make_pair(((Value*)new Number(val, FLOAT))->setContext(this->context), nullptr);
        return std::make_pair(((Value*)new Number(int(val)))->setContext(this->context), nullptr);
    }
    else if (other->type == STRING && this->type == STRING){
        std::string newString = ((String*)this)->value + ((String*)other)->value;
        return std::make_pair(((Value*)new String(newString))->setContext(this->context), nullptr);
    }
    else if (this->type == LIST)
    {
        auto newList = (List*)this;
        newList->elements.push_back(other);
        return std::make_pair(newList, nullptr);
    }
    
    return std::make_pair(nullptr, IllegalOperation(other));;
}
std::pair<Value*, Error*> Value::subtractedBy(Value* other) {
    if ((other->type == INTEGER || other->type == FLOAT)&&(this->type == INTEGER || this->type == FLOAT)){
        double val = ((Number*)this)->value - ((Number*)other)->value;
        if (this->type == FLOAT || other->type == FLOAT)
            return std::make_pair(((Value*)new Number(val, FLOAT))->setContext(this->context), nullptr);
        return std::make_pair(((Value*)new Number(int(val)))->setContext(this->context), nullptr);
    }
    else if (this->type == LIST && (other->type == INTEGER || other->type == FLOAT)){
        auto newList = (List*)this;
        if (newList->elements.size() > ((Number*)other)->value){
            newList->elements.erase(newList->elements.begin()+int(((Number*)other)->value));
            return std::make_pair(newList, nullptr);
        }
        else
        {
            return std::make_pair(nullptr, (Error*)new RuntimeError(posStart->filename, *posStart, *posEnd, 
                "Element at this index could not be removed because index is out of bounds", context));
        }}
    return std::make_pair(nullptr, IllegalOperation(other));;
}
std::pair<Value*, Error*> Value::multipliedby(Value* other) {
    if ((other->type == INTEGER || other->type == FLOAT)&&(this->type == INTEGER || this->type == FLOAT)){
        double val = ((Number*)this)->value * ((Number*)other)->value;
        if (this->type == FLOAT || other->type == FLOAT)
            return std::make_pair((new Number(val, FLOAT))->setContext(this->context), nullptr);
        return std::make_pair((new Number(int(val)))->setContext(this->context), nullptr);
    }
    else if ((other->type == STRING && this->type == INTEGER) || (other->type == INTEGER && this->type == STRING)){
        std::string newString = ((String*)this)->value + ((String*)other)->value;
        return std::make_pair(((Value*)new String(newString))->setContext(this->context), nullptr);
    }
    else if (this->type == LIST && other->type == LIST)
    {
        auto newList = (List*)this;
        newList->elements.insert(newList->elements.end(), ((List*)other)->elements.begin(), ((List*)other)->elements.end() );
        return std::make_pair(newList, nullptr);
    }
    
    return std::make_pair(nullptr, IllegalOperation(other));;
}
std::pair<Value*, Error*> Value::dividedby(Value* other) {
    if ((other->type == INTEGER || other->type == FLOAT)&&(this->type == INTEGER || this->type == FLOAT)){
        double val = ((Number*)this)->value / ((Number*)other)->value;
        if (((Number*)other)->value == 0)
            return std::make_pair(nullptr, (Error*)(new RuntimeError(other->posStart->filename, *other->posStart, *other->posEnd, "Division by zero", context)));
        return std::make_pair((new Number(val, FLOAT))->setContext(this->context), nullptr);
    }
    else if (this->type == LIST && (other->type == INTEGER || other->type == FLOAT)){
    if (((List*)this)->elements.size() > ((Number*)other)->value){
        return std::make_pair(((List*)this)->elements[int(((Number*)other)->value)], nullptr);
    }
    else
    {
        return std::make_pair(nullptr, (Error*)new RuntimeError(posStart->filename, *posStart, *posEnd, 
            "Element at this index could not be retrieved because index is out of bounds", context));
    }
}
    
    return std::make_pair(nullptr, IllegalOperation(other));;
}
std::pair<Value*, Error*> Value::powedby(Value* other) {
    if ((other->type == INTEGER || other->type == FLOAT)&&(this->type == INTEGER || this->type == FLOAT)){
        double val = pow(((Number*)this)->value, ((Number*)other)->value);
        return std::make_pair((new Number(val, FLOAT))->setContext(this->context), nullptr);
    }
    return std::make_pair(nullptr, IllegalOperation(other));
}

Value* Value::setContext(Context* context = nullptr){
    this->context = context;
    return this;
}

Value* RuntimeResult::registerResult(RuntimeResult* res){
    if (res->error)
        error = res->error;
    return res->value;
}
RuntimeResult* RuntimeResult::success(Value* value){
    if (value){
    this->value = value;
    this->type = value->type;
    }
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

void SymbolTable::set(std::string id, Value* val){
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

Value* SymbolTable::get(std::string id){
    std::size_t index = symHash(id);
    auto atIndex = symtab[index];
    if (atIndex){
        while (atIndex->name != id && atIndex->next)
            atIndex = atIndex->next;
        if (atIndex->name == id){
            return atIndex->value;
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

std::pair<Value*, Error*> Value::getComparisonEQ(Value* other){
    if (other->type == INTEGER || other->type == FLOAT){
        double val = ((Number*)this)->value == ((Number*)other)->value;
        return std::make_pair((new Number(val, INTEGER))->setContext(this->context), nullptr);
    }
    return std::make_pair(nullptr, IllegalOperation(other));
}
std::pair<Value*, Error*> Value::getComparisonNotEq(Value* other){
    if (other->type == INTEGER || other->type == FLOAT){
        double val = ((Number*)this)->value != ((Number*)other)->value;
        return std::make_pair((new Number(val, INTEGER))->setContext(this->context), nullptr);
    }
    return std::make_pair(nullptr, IllegalOperation(other));
}
std::pair<Value*, Error*> Value::getComparisonLT(Value* other){
    if (other->type == INTEGER || other->type == FLOAT){
        double val = ((Number*)this)->value < ((Number*)other)->value;
        return std::make_pair((new Number(val, INTEGER))->setContext(this->context), nullptr);
    }
    return std::make_pair(nullptr, IllegalOperation(other));
}
std::pair<Value*, Error*> Value::getComparisonGT(Value* other){
    if (other->type == INTEGER || other->type == FLOAT){
        double val = ((Number*)this)->value > ((Number*)other)->value;
        return std::make_pair((new Number(val, INTEGER))->setContext(this->context), nullptr);
    }
    return std::make_pair(nullptr, IllegalOperation(other));
}
std::pair<Value*, Error*> Value::getComparisonLTE(Value* other){
    if (other->type == INTEGER || other->type == FLOAT){
        double val = ((Number*)this)->value <= ((Number*)other)->value;
        return std::make_pair((new Number(val, INTEGER))->setContext(this->context), nullptr);
    }
    return std::make_pair(nullptr, IllegalOperation(other));
}
std::pair<Value*, Error*> Value::getComparisonGTE(Value* other){
    if (other->type == INTEGER || other->type == FLOAT){
        double val = ((Number*)this)->value >= ((Number*)other)->value;
        return std::make_pair((new Number(val, INTEGER))->setContext(this->context), nullptr);
    }
    return std::make_pair(nullptr, IllegalOperation(other));
}
std::pair<Value*, Error*> Value::logicalAnd(Value* other){
    if (other->type == INTEGER || other->type == FLOAT){
        double val = ((Number*)this)->value && ((Number*)other)->value;
        return std::make_pair((new Number(val, INTEGER))->setContext(this->context), nullptr);
    }
    return std::make_pair(nullptr, IllegalOperation(other));
}
std::pair<Value*, Error*> Value::logicalOr(Value* other){
    if (other->type == INTEGER || other->type == FLOAT){
        double val = ((Number*)this)->value || ((Number*)other)->value;
        return std::make_pair((new Number(val, INTEGER))->setContext(this->context), nullptr);
    }
    return std::make_pair(nullptr, IllegalOperation(other));
}
std::pair<Value*, Error*> Value::logicalNot(){
    double val = !(((Number*)this)->value);
    return std::make_pair((new Number(val, INTEGER))->setContext(this->context), nullptr);
}

bool Value::isTrue(){
    return ((Number*)this)->value != 0;
}

Error* Value::IllegalOperation(Value* other){
    if (!other)
        other = this;
    return (Error*)new RuntimeError(other->posStart->filename, *other->posStart, *other->posEnd, "Illegal operation", other->context);
}

RuntimeResult* Function::execute(std::vector<Value*> args){
    auto res = new RuntimeResult();
    auto interpreter = new Interpreter();
    Context* newContext = new Context(name, context, posStart);
    newContext->symTab = new SymbolTable(100, newContext->parent->symTab);

    if (args.size() > argNames.size())
        return res->failure((Error*)new RuntimeError(posStart->filename, *posStart, *posEnd, (args.size() - argNames.size() + " too many args into " + name), context));
    if (args.size() < argNames.size())
        return res->failure((Error*)new RuntimeError(posStart->filename, *posStart, *posEnd, (argNames.size() - args.size() + " too few args into " + name), context));

    for (int i = 0; i < args.size(); i++){
        auto argName = argNames[i];
        auto argValue = args[i];
        argValue->setContext(newContext);
        newContext->symTab->set(argName, argValue);
    }
    auto value = res->registerResult(interpreter->visit(BodyNode, newContext));
    if (res->error) return res;

    return res->success(value);
}

std::ostream& operator<<(std::ostream& os , const Function* func){
    return os << "<function " + func->name + ">";
}

Value* copyValue(Value* val){
    switch (val->type)
    {
    case INTEGER: case FLOAT:
        return new Number(((Number*)val)->value, val->type);
    case FUNCDEF:
        return new Function(((Function*)val));
    case LIST:
        return new List(((List*)val));
    default:
        return nullptr;
    }
}

void Lexer::reset(std::string sc, std::string fn){
    this->filename = fn;
    this->sourceCode = sc;
    this->it = sourceCode.begin();
    this->pos.set(fn, sc);
    tokens.clear();
}

void Interpreter::setNode(astNode* node){
    this->node = node;
}