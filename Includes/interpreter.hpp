#pragma once

#include "nodes.hpp"
#include <vector>
#include <string>

class RuntimeResult;
class astNode;
class Context;
class Error;

class Value{
public:
    Type type;
    Position* posStart;
    Position* posEnd;
    Context* context;
    Value(Type t) : type(t) {setPos();};
    Value(Value* val) : type(val->type), posStart((val->posStart)?new Position(*val->posStart):nullptr), posEnd((val->posEnd)?new Position(*val->posEnd):nullptr), context(val->context) {};
    Value* setPos(Position* start = nullptr, Position* end = nullptr);
    Value* setContext(Context* context);
    std::pair<Value*, Error*> addedTo(Value* other);
    std::pair<Value*, Error*> subtractedBy(Value* other);
    std::pair<Value*, Error*> multipliedby(Value* other);
    std::pair<Value*, Error*> dividedby(Value* other);
    std::pair<Value*, Error*> powedby(Value* other);
    std::pair<Value*, Error*> getComparisonEQ(Value* other);
    std::pair<Value*, Error*> getComparisonNotEq(Value* other);
    std::pair<Value*, Error*> getComparisonLT(Value* other);
    std::pair<Value*, Error*> getComparisonGT(Value* other);
    std::pair<Value*, Error*> getComparisonLTE(Value* other);
    std::pair<Value*, Error*> getComparisonGTE(Value* other);
    std::pair<Value*, Error*> logicalAnd(Value* other);
    std::pair<Value*, Error*> logicalOr(Value* other);
    std::pair<Value*, Error*> logicalNot();
    bool isTrue();
    Error* IllegalOperation(Value* other);
};

class Number : public Value{
public:
    double value;
    Number(double val, Type t = INTEGER) : value(val), Value(t) {setPos();};
    Number(Number* num) : value(num->value), Value(num) {};
};

class String : public Value{
public:
    std::string value;
    String(std::string val, Type t = STRING) : value(val), Value(t) {setPos();};
    String(String* num) : value(num->value), Value(num) {};
};

class Function : public Value{
public:
    std::string name;
    astNode* BodyNode;
    std::vector<std::string> argNames;
    Function(std::string name, astNode* body, std::vector<std::string> argNames) : name(name), BodyNode(body), argNames(argNames), Value(FUNCDEF) {setPos();};
    Function(Function* fun) : name(fun->name), BodyNode(fun->BodyNode), argNames(fun->argNames), Value(fun) {setPos();};
    RuntimeResult* execute(std::vector<Value*> argNames);
    friend std::ostream& operator<<(std::ostream& os , const Function* func);
};

class SymNode{
public:
    std::string name;
    Value* value;
    SymNode* next;
    SymNode(std::string id, Value* val) : name(id), value(val), next(nullptr) {};
};

class SymbolTable{
public:
    std::vector<SymNode*> symtab;
    std::size_t numOfSyms; // for when I have time to implement dynamic size tables
    SymbolTable* parent;
    unsigned int symHash(std::string id);
    Value* get(std::string id);
    void set(std::string id, Value* val);
    SymbolTable(std::size_t tableSize, SymbolTable* parent = nullptr) 
        {for (std::size_t i = 0; i < tableSize; i++) symtab.push_back(nullptr); this->parent = parent; numOfSyms = 0;}
    ~SymbolTable() {
                        for (std::size_t i = 0; i < symtab.size(); i++)
                            if (symtab[i]){
                                auto next = symtab[i]->next;
                                while (next){
                                    auto cpy = next;
                                    next = next->next;
                                    delete cpy;
                                }
                                delete symtab[i];
                            }
                    }
};

class Context{
public:
    std::string displayName;
    class Context* parent;
    class Position* parentEntryPos;
    class SymbolTable* symTab;
    Context(std::string dn, Context* c = nullptr, Position* p = nullptr) : displayName(dn), parent(c), parentEntryPos(p), symTab(nullptr) {};
};

enum errorType{
    ILLEGALCHAR,
    SYNTAXERROR,
    RTERROR
};

class Error{
public:
    errorType type;
    std::string errorName;
    std::string fileName;
    std::string details;
    Position posStart;
    Position posEnd;
    Error(errorType t, std::string errorName, std::string fn, std::string details, Position start, Position end) : 
        type(t), errorName(errorName), fileName(fn), details(details), posStart(start), posEnd(end) {};
    std::string toString();
};

class IllegalCharError : Error
{
public:
    IllegalCharError(std::string fn, std::string details, Position posStart, Position posEnd) : Error(ILLEGALCHAR ,"Illegal character", fn, details, posStart, posEnd) {};
    ~IllegalCharError();
};

class InvalidSyntaxError : Error
{
public:
    InvalidSyntaxError(std::string fn, Position posStart, Position posEnd, std::string details = "") : Error(SYNTAXERROR, "Invalid Syntax", fn, details, posStart, posEnd) {};
    ~InvalidSyntaxError();
};

class RuntimeError : Error
{
public:
    Context* context;
    RuntimeError(std::string fn, Position posStart, Position posEnd, std::string details = "", Context* context = nullptr) : Error(RTERROR, "Runtime Error", fn, details, posStart, posEnd), context(context) {};
    ~RuntimeError();
    std::string toString();
    std::string generateTraceback();
};

class Lexer{
    std::string sourceCode;
    Position pos;
    std::string filename;
    std::string::iterator it;
    std::vector<struct Token> tokens;
    void advance();
public:
    Lexer(std::string sc, std::string fn) : sourceCode(sc), pos(fn, sourceCode), filename(fn), it(sourceCode.begin()) {};
    std::pair<std::vector<struct Token>, Error*> makeTokens();
};

class Interpreter{
public:
    astNode* node;
    RuntimeResult* visit(astNode* node, Context* context);
    RuntimeResult* visitInteger(astNode* node, Context* context);
    RuntimeResult* visitFloat(astNode* node, Context* context);
    RuntimeResult* visitBinNode(astNode* node, Context* context);
    RuntimeResult* visitUnNode(astNode* node, Context* context);
    RuntimeResult* visitVarAccessNode(astNode* node, Context* context);
    RuntimeResult* visitVarAssignNode(astNode* node, Context* context);
    RuntimeResult* visitIfNode(astNode* node, Context* context);
    RuntimeResult* visitForNode(astNode* node, Context* context);
    RuntimeResult* visitWhileNode(astNode* node, Context* context);
    RuntimeResult* visitFuncDefNode(astNode* node, Context* context);
    RuntimeResult* visitCallNode(astNode* node, Context* context);
    RuntimeResult* visitString(astNode* node, Context* context);
    Interpreter(astNode* n) : node(n) {};
    Interpreter() : node(nullptr) {};
};

class RuntimeResult{
public:
    Type type;
    Value* value;
    Error* error;
    RuntimeResult() : type(INVALID), value(nullptr), error(nullptr) {};
    Value* registerResult(RuntimeResult* value);
    RuntimeResult* success(Value* value);
    RuntimeResult* failure(Error* error);
};

Value* copyValue(Value* val);