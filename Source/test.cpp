#include <iostream>
#include <string>
#include "test.hpp"
#include "parser.hpp"
#include "interpreter.hpp"

static SymbolTable symtab(10); 

bool testCode(std::string code, double expectedResult){
    Lexer lex(code, "Test module");
    auto tokens = lex.makeTokens();

    if(tokens.second){
        return false;
    }

    Parser p(tokens.first);
    auto res = p.parse();
    if (res->error){
            return false;
    }

    Interpreter interpreter(res->node);
    Context* context = new Context("<program>");
    context->symTab = &symtab;
    auto result = interpreter.visit(res->node, context);
    if (result->error) 
        return false;
    if (result->type == INTEGER || result->type == FLOAT)
        return ((Number*)(result->value))->value == expectedResult;
    return false;
}

bool testAllFunc(){
    int testNum = 1;
    std::cout << "Test " << testNum << " - Numbers and arithmetic operations:" << std::endl;
    if(testArithm())
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;
    return true;
}

bool testArithm(){
    bool success = true;
    std::cout << "\tTesting basic operations ('+', '-', '*', '/' and '^'):" << std::endl;;
    std::string code[5]{"5 + 5", "33 - 10", "3 * 3", "10 / 4", "25 ^ 0.5"};
    double expVal[5]{10, 23, 9, 2.5, 5};
    for (int i = 0; i < 5; i++){
        std::cout << "\t\t" << code[i] << " = " << expVal[i] << ":";
        if (testCode(code[i], expVal[i]))
            std::cout << " Success" << std::endl;
        else
        {
            std::cout << " Failed" << std::endl;
            success = false;
        }
    }
    return success;
}
/* 
bool testCompareAndLogic(){

}

bool testIfStat(){

}

bool testForLoop(){

}

bool testWhileLoop(){

}

bool testSingleLineFunction(){

}

bool testMultilineFunction(){

}

bool testList(){

}

bool testStrings(){

}

bool testVariables(){

}

bool testBuiltInFunctions(){

}

bool testMultiLineStat(){

}

bool testReturnStat(){

}

bool testReadScript(){

}
 */