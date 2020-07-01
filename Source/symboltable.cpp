#include <string>
#include <cmath>
#include "symboltable.hpp"


unsigned int SymbolTable::symHash(std::string id){
    unsigned int val = 0;
    short i = 1;
    for (char& c: id){
        val += pow(c, i);
        i++;
    }
    return val%symtab.size();
}

void SymbolTable::set(std::string id, Number* val){
    std::size_t index = symHash(id);
    auto atIndex = symtab[index];
    if (atIndex){
        while (atIndex->name != id && atIndex->next)
            atIndex = atIndex->next;
        if (atIndex->name == id){
            atIndex->value = val;
        }
        else if (parent)
            parent->set(id, val);
        else{
            atIndex->next = new SymNode(id, val);
            numOfSyms++;
        }
    }
    else{
        symtab[index] = new SymNode(id, val);
    }
}

Number* SymbolTable::get(std::string id){
    std::size_t index = symHash(id);
    auto atIndex = symtab[index];
    if (atIndex){
        while (atIndex->name != id && atIndex->next)
            atIndex = atIndex->next;
        if (atIndex->name == id){
            return &atIndex->value;
        }
        else if (parent)
            return parent->get(id);
    }
    return nullptr;