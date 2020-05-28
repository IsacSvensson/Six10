#ifndef TYPECHECKERS_HPP
#define TYPECHECKERS_HPP

#include <string>
#include "lexer.hpp"

bool isParentheses(std::string::iterator it);
bool isKeyword(std::string str);
bool isDataType(std::string str);
bool isAssignmentOperator(std::string str);
bool isArithmeticOperator(std::string str);
std::pair<Type, int> isType(std::string::iterator it);

#endif