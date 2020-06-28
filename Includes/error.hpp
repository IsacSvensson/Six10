#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>
#include "position.hpp"
#include "context.hpp"

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

#endif