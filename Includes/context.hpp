#pragma once 

#include "position.hpp"
#include "interpreter.hpp"
#include "symboltable.hpp"

class Context{
public:
    std::string displayName;
    class Context* parent;
    class Position* parentEntryPos;
    class SymbolTable symTab;
    Context(std::string dn, Context* c = nullptr, Position* p = nullptr) : displayName(dn), parent(c), parentEntryPos(p), symTab(100) {};
};
