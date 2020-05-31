#ifndef TYPECHECKERS_HPP
#define TYPECHECKERS_HPP

#include <string>
#include "lexer.hpp"

bool isParentheses(std::string::iterator it);
bool isAssignmentOperator(std::string str);
bool islogicalOperator(std::string str);
bool isRelationalOperator(std::string str);
bool isArithmeticOperator(std::string str);
bool isKeyword(std::string str);
bool isDataType(std::string str);
std::string getType(const Token t);
std::pair<Type, int> isType(std::string::iterator it, std::string::iterator end);

#endif


