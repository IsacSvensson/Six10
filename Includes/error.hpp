#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>
#include "position.hpp"

class Error{
    std::string errorName;
    std::string fileName;
    std::string details;
    Position posStart;
    Position posEnd;
public:
    Error(std::string errorName, std::string details, Position start, Position end) : 
        errorName(errorName), details(details), posStart(start), posEnd(end) {};
    std::string toString();
};

class IllegalCharError : Error
{
public:
    IllegalCharError(std::string details, Position posStart, Position posEnd) : Error("Illegal character", details, posStart, posEnd) {};
    ~IllegalCharError();
};

#endif