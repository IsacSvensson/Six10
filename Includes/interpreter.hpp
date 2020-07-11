#pragma once

#include "nodes.hpp"
#include <vector>
#include <string>

class RuntimeResult;
class astNode;
class Context;
class Error;

class Number{
public:
    Type type;
    double value;
    Position* posStart;
    Position* posEnd;
    Context* context;
    Number(double val, Type t = INTEGER) : type(t), value(val) {setPos();};
    Number(Number* num) : type(num->type), value(num->value), posStart((num->posStart)?new Position(*num->posStart):nullptr), posEnd((num->posEnd)?new Position(*num->posEnd):nullptr) {};
    Number* setPos(Position* start = nullptr, Position* end = nullptr);
    Number* setContext(Context* context);
    std::pair<Number*, Error*> addedTo(Number* other);
    std::pair<Number*, Error*> subtractedBy(Number* other);
    std::pair<Number*, Error*> multipliedby(Number* other);
    std::pair<Number*, Error*> dividedby(Number* other);
    std::pair<Number*, Error*> powedby(Number* other);
};

class SymNode{
public:
    std::string name;
    Number value;
    SymNode* next;
    SymNode(std::string id, Number* val) : name(id), value(val), next(nullptr) {};
};

class SymbolTable{
public:
    std::vector<SymNode*> symtab;
    std::size_t numOfSyms; // for when I have time to implement dynamic size tables
    SymbolTable* parent;
    unsigned int symHash(std::string id);
    Number* get(std::string id);
    void set(std::string id, Number* val);
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
