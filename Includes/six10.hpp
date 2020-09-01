#pragma once 

#include "interpreter.hpp"
#include "parser.hpp"

class Six10
{
private:
    SymbolTable* symtab;
    Parser parser;
    Lexer lexer;
    Interpreter interpreter;
public:
    Six10(SymbolTable* symtab = new SymbolTable());
    ~Six10();
    std::pair<Value*, Error*> run(std::string code, std::string fn);
};