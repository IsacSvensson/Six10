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
    Context context("<program>");
    context.symTab = &symtab;
    auto result = interpreter.visit(res->node, &context);
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
    symtab.set("null", new Number(0, INTEGER));
    symtab.set("True", new Number(1, INTEGER));
    symtab.set("False", new Number(0, INTEGER));
    int testNum = 1;
    int successfulTests = 0;
    std::cout << "Test " << testNum << " - Numbers and arithmetic operations:" << std::endl;
    if(testArithm()){
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
        successfulTests++;
    }
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;

    std::cout << "\nTest " << testNum << " - Comparison Operations and Logic Operators:" << std::endl;
    if(testCompareAndLogic()){
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
        successfulTests++;
    }
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;

    std::cout << "\nTest " << testNum << " - If-Statments:" << std::endl;
    if(testIfStat()){
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
        successfulTests++;
    }
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;

    std::cout << "\nTest " << testNum << " - For-Loops:" << std::endl;
    if(testForLoop()){
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
        successfulTests++;
    }
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;

    std::cout << "\nTest " << testNum << " - While-Loops:" << std::endl;
    if(testWhileLoop()){
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
        successfulTests++;
    }
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;

    std::cout << "\nTest " << testNum << " - Single Line Function:" << std::endl;
    if(testSingleLineFunction()){
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
        successfulTests++;
    }
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;

    std::cout << "\nTest " << testNum << " - Multi Line Function:" << std::endl;
    if(testMultilineFunction()){
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
        successfulTests++;
    }
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;

    std::cout << "\nTest " << testNum << " - Lists:" << std::endl;
    if(testList()){
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
        successfulTests++;
    }
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;

    std::cout << "\nTest " << testNum << " - Strings:" << std::endl;
    if(testStrings()){
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
        successfulTests++;
    }
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;

    std::cout << "\nTest " << testNum << " - Variables:" << std::endl;
    if(testVariables()){
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
        successfulTests++;
    }
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;

    std::cout << "\nTest " << testNum << " - Built-In Functions:" << std::endl;
    if(testBuiltInFunctions()){
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
        successfulTests++;
    }
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;

    std::cout << "\nTest " << testNum << " - Multi Line Statments:" << std::endl;
    if(testMultiLineStat()){
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
        successfulTests++;
    }
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;

    std::cout << "\nTest " << testNum << " - Return Statments:" << std::endl;
    if(testReturnStat()){
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
        successfulTests++;
    }
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;

    std::cout << "\nTest " << testNum << " - Reading files:" << std::endl;
    if(testReadScript()){
        std::cout << "Result - Test " << testNum++ << ": Success" << std::endl;
        successfulTests++;
    }
    else
        std::cout << "Result - Test " << testNum++ << ": Failed" << std::endl;

    std::cout << '\n' << successfulTests << "/" << testNum-1 << " tests successful!" << std::endl;
    return testNum == successfulTests;
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
    std::string codeIO[3]{"5 / 0", "(10 / (5 - 5)) + 10", "0 / 0"};
    double expValIO[3] = {-9999, -9999, -9999};
    success = true;
    for (int i = 0; i < 3; i++){
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
        std::cout << "\tTest 1.4 - Success" << std::endl;
    else
        std::cout << "\tTest 1.4 - Fail" << std::endl;
    return finalSuccess;
}

bool testCompareAndLogic(){
    bool finalSuccess = true;
    std::cout << "\tTest 2.1 - Comparison Operators ('<', '>', '<=', '>=', '==', and '!='):" << std::endl;
    std::string code[18]{"5 < 5", "5 < -1", "5 < 10", "5 > 5", "5 > -1", "5 > 10", "5 <= 5", "5 <= -1", "5 <= 10", 
        "5 >= 5", "5 >= -1", "5 >= 10", "5 == 5", "5 == -1", "5 == 10", "5 != 5", "5 != -1", "5 != 10"};
    double expVal[18]{0,0,1,0,1,0,1,0,1,1,1,0,1,0,0,0,1,1};
    bool success = true;
    for (int i = 0; i < 18; i++){
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
        std::cout << "\tTest 2.1 - Success\n" << std::endl;
    else
        std::cout << "\tTest 2.1 - Fail\n" << std::endl;
    std::cout << "\tTest 2.2 - Logic Operators ('and'/'&&', 'or'/'||' and 'not'/'!'):" << std::endl;
    std::string codeLogic[16]{"True and False", "True and True", "False and False", "True && False", "True && True", "False && False", "True or False",
        "True or True", "False or False", "True || False", "True || True", "False || False", "not False", "not True", "!False", "!True"};
    double expValLogic[16]{0,1,0,0,1,0,1,1,0,1,1,0,1,0,1,0};
    success = true;
    for (int i = 0; i < 16; i++){
        std::cout << "\t\t" << codeLogic[i] << " = " << expValLogic[i] << ":";
        if (testCode(codeLogic[i], expValLogic[i]))
            std::cout << " Success" << std::endl;
        else
        {
            std::cout << " Failed" << std::endl;
            finalSuccess = success = false;
        }
    }
    if (success)
        std::cout << "\tTest 2.2 - Success" << std::endl;
    else
        std::cout << "\tTest 2.2 - Fail" << std::endl;
    return finalSuccess;
}

bool testIfStat(){
    return false;
}

bool testForLoop(){
    return false;
}

bool testWhileLoop(){
    return false;
}

bool testSingleLineFunction(){
    return false;
}

bool testMultilineFunction(){
    return false;
}

bool testList(){
    return false;
}

bool testStrings(){
    return false;
}

bool testVariables(){
    return false;
}

bool testBuiltInFunctions(){
    return false;
}

bool testMultiLineStat(){
    return false;
}

bool testReturnStat(){
    return false;
}

bool testReadScript(){
    return false;
}