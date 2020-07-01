#pragma once

#include <vector>
#include <string>
#include "interpreter.hpp"

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
