#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>
#include "position.hpp"

class Error{
public:
    std::string errorName;
    std::string fileName;
    std::string details;
    Position posStart;
    Position posEnd;
    Error(std::string errorName, std::string fn, std::string details, Position start, Position end) : 
        errorName(errorName), fileName(fn), details(details), posStart(start), posEnd(end) {};
    std::string toString();
};

class IllegalCharError : Error
{
public:
    IllegalCharError(std::string fn, std::string details, Position posStart, Position posEnd) : Error("Illegal character", fn, details, posStart, posEnd) {};
    ~IllegalCharError();
};

class InvalidSyntaxError : Error
{
public:
    InvalidSyntaxError(std::string fn, Position posStart, Position posEnd, std::string details = "") : Error("Invalid Syntax", fn, details, posStart, posEnd) {};
    ~InvalidSyntaxError();
};

#endif