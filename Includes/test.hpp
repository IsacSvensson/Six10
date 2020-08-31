#include <string>
#include "interpreter.hpp"

bool testCode(std::string code, double expectedResult, SymbolTable* symtab);
bool testCode(std::string code, std::string expectedResult, SymbolTable* symtab);
bool testAllFunc(SymbolTable* symtab);
bool testArithm(SymbolTable* symtab);
bool testCompareAndLogic(SymbolTable* symtab);
bool testIfStat(SymbolTable* symtab);
bool testForLoop(SymbolTable* symtab);
bool testWhileLoop(SymbolTable* symtab);
bool testFunction(SymbolTable* symtab);
bool testList(SymbolTable* symtab);
bool testStrings(SymbolTable* symtab);
bool testVariables(SymbolTable* symtab);
bool testBuiltInFunctions(SymbolTable* symtab);
bool testReturnStat(SymbolTable* symtab);
bool testReadScript(SymbolTable* symtab);