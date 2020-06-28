#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "position.hpp"

class Context{
public:
    std::string displayName;
    class Context* parent;
    class Position* parentEntryPos;
    Context(std::string dn, Context* c = nullptr, Position* p = nullptr) : displayName(dn), parent(c), parentEntryPos(p) {};
};

#endif