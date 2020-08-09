#include <iostream>
#include <string>
#include <cmath>
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
    if (result->error){
        if (expectedResult == -9999)
            return true;
        return false;
    }
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
    bool finalSuccess = true;
    std::cout << "\tTest 1.1 - Basic Operations ('+', '-', '*', '/' and '^'):" << std::endl;;
    std::string code[5]{"5 + 5", "33 - 10", "3 * 3", "10 / 4", "5 ^ 2"};
    double expVal[5]{10, 23, 9, 2.5, 25};
    bool success = true;
    for (int i = 0; i < 5; i++){
        std::cout << "\t\t" << code[i] << " = " << expVal[i] << ":";
        if (testCode(code[i], expVal[i]))
            std::cout << " Success" << std::endl;
        else
        {
            std::cout << " Failed" << std::endl;
            finalSuccess = success = false;
        }
    }
    if (success)
        std::cout << "\tTest 1.1 - Success\n" << std::endl;
    else
        std::cout << "\tTest 1.1 - Fail\n" << std::endl;

    std::cout << "\tTest 1.2 - Multiple Operations:" << std::endl;;
    std::string codeMO[5]{"5 + 5 - 3", "5*9-15/3", "(3 * 3)*3", "(10 / 4 * 4 + 15) ^ 0.5", "(6*6)/(2*2)"};
    double expValMO[5] = {7, 40, 27, 5, 9};
    success = true;
    for (int i = 0; i < 5; i++){
        std::cout << "\t\t" << codeMO[i] << " = " << expValMO[i] << ":";
        if (testCode(codeMO[i], expValMO[i]))
            std::cout << " Success" << std::endl;
        else
        {
            std::cout << " Failed" << std::endl;
            finalSuccess = success = false;
        }
    }
    if (success)
        std::cout << "\tTest 1.2 - Success\n" << std::endl;
    else
        std::cout << "\tTest 1.2 - Fail\n" << std::endl;

    std::cout << "\tTest 1.3 - Negative numbers:" << std::endl;;
    std::string codeNN[5]{"-5 + 5", "-3 * 7", "-255 / -5", "-10 ^ 3", "-1"};
    double expValNN[5] = {0, -21, 51, -1000, -1};
    success = true;
    for (int i = 0; i < 5; i++){
        std::cout << "\t\t" << codeNN[i] << " = " << expValNN[i] << ":";
        if (testCode(codeNN[i], expValNN[i]))
            std::cout << " Success" << std::endl;
        else
        {
            std::cout << " Failed" << std::endl;
            finalSuccess = success = false;
        }
    }
    if (success)
        std::cout << "\tTest 1.3 - Success\n" << std::endl;
    else
        std::cout << "\tTest 1.3 - Fail\n" << std::endl;

    std::cout << "\tTest 1.4 - Illegal Operations:" << std::endl;;
    std::string codeIO[2]{"5 / 0", "(10 / (5 - 5)) + 10"};
    double expValIO[2] = {-9999, -9999};
    success = true;
    for (int i = 0; i < 2; i++){
        std::cout << "\t\t" << codeIO[i] << " = " << "Runtime Error: Division by 0 not allowed" << ":";
        if (testCode(codeIO[i], expValIO[i]))
            std::cout << " Success" << std::endl;
        else
        {
            std::cout << " Failed" << std::endl;
            finalSuccess = success = false;
        }
    }
    if (success)
        std::cout << "\tTest 1.4 - Success\n" << std::endl;
    else
        std::cout << "\tTest 1.4 - Fail\n" << std::endl;
    return finalSuccess;
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