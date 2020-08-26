#pragma once

#include <string>
#include "interpreter.hpp"

std::string stringWithArrows(std::string text, Position* start, Position* end);
bool isParentheses(std::string::iterator it);
bool isAssignmentOperator(std::string str);
bool islogicalOperator(std::string str);
bool isRelationalOperator(std::string str);
bool isArithmeticOperator(std::string str);
bool isKeyword(std::string str);
bool isDataType(std::string str);
std::string getType(const Token t);
std::string getType(Type t);
std::pair<Type, int> isType(std::string::iterator it, std::string::iterator end);
void getSourceCode(std::string path, std::string &sourceCode);
std::string printValue(Value* res);